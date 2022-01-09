#include "EnginePCH.h"
#include "Input.h"

#include <chrono>

namespace Engine
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;

	static const float MIN_KEY_HELD_TIME = 1.0f / 30.0f;
	static const float MIN_MOUSE_HELD_TIME = 1.0f / 50.0f;
	static const float MAX_SCROLL_TIME = 1.0f / 30.0f;

	
	InputMouseMoveEvent::InputMouseMoveEvent(const MathLib::Vector2& mousePos)
		: mousePos(mousePos), prevMousePos(Input::GetMouseScreenPos()) { }


	static Timestep CalculateTimestepDiff(const TimePoint& prevTime)
	{
		TimePoint current = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds diff =
			std::chrono::duration_cast<std::chrono::nanoseconds>(current - prevTime);
		return (diff.count() * 0.000000001f);
	}

	struct InputKeyCodeData
	{
		bool isPressed;
		TimePoint lastTimePressed;
	};

	struct InputMouseData
	{
		struct MouseButtonData
		{
			TimePoint lastTimeClicked;
			InputAction buttonState;
		};

		MouseButtonData rightButton;
		MouseButtonData leftButton;
		MouseButtonData middleButton;

		MathLib::Vector2 screenPos;

		MathLib::Vector2 scrollOffset;
		TimePoint scrollTime;
	};

	static std::unordered_map<InputKeyCode, InputKeyCodeData> s_keyCodes
		= std::unordered_map<InputKeyCode, InputKeyCodeData>();
	static InputMouseData s_mouseInputData;

	Input::EventFunc s_inputEventFunc = nullptr;

	void Input::BindInputEventFunc(const Input::EventFunc& func)
	{
		s_inputEventFunc = func;
	}

	void Input::OnEvent(Event& event)
	{
		EventDispatcher eventDispatcher(event);
		eventDispatcher.Invoke<InputKeyEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputKeyEvent));
		eventDispatcher.Invoke<InputMouseButtonEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseButtonEvent));
		eventDispatcher.Invoke<InputMouseMoveEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseMoveEvent));
		eventDispatcher.Invoke<InputMouseScrollEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseScrollEvent));
	}

	bool Input::IsKeyPressed(InputKeyCode keyCode)
	{
		const auto& found = s_keyCodes.find(keyCode);
		if (found != s_keyCodes.end()
			&& found->second.isPressed)
		{
			return GetTimeHeld(keyCode) < MIN_KEY_HELD_TIME;
		}
		return false;
	}
	
	bool Input::IsKeyHeld(InputKeyCode keyCode)
	{
		return GetTimeHeld(keyCode) >= MIN_KEY_HELD_TIME;
	}

	float Input::GetTimeHeld(InputKeyCode keyCode)
	{
		const auto& found = s_keyCodes.find(keyCode);
		if (found != s_keyCodes.end()
			&& found->second.isPressed)
		{
			return CalculateTimestepDiff(found->second.lastTimePressed);
		}
		return 0.0f;
	}

	bool Input::IsMouseButtonPressed(InputMouseButton button)
	{
		InputMouseData::MouseButtonData* mouseButtonData = nullptr;
		switch (button)
		{
		case MOUSE_BUTTON_LEFT:
		{
			mouseButtonData = &s_mouseInputData.leftButton;
			break;
		}
		case MOUSE_BUTTON_MIDDLE:
		{
			mouseButtonData = &s_mouseInputData.rightButton;
			break;
		}
		case MOUSE_BUTTON_RIGHT:
		{
			mouseButtonData = &s_mouseInputData.middleButton;
			break;
		}
		}
		if (mouseButtonData == nullptr) return false;
		Timestep outputDiff = CalculateTimestepDiff(mouseButtonData->lastTimeClicked);
		return mouseButtonData->buttonState == ACTION_PRESSED
			&& outputDiff.GetSeconds() < MIN_MOUSE_HELD_TIME;
	}

	bool Input::IsMouseButtonHeld(InputMouseButton button)
	{
		InputMouseData::MouseButtonData* mouseButtonData = nullptr;
		switch (button)
		{
		case MOUSE_BUTTON_LEFT:
		{
			mouseButtonData = &s_mouseInputData.leftButton;
			break;
		}
		case MOUSE_BUTTON_MIDDLE:
		{
			mouseButtonData = &s_mouseInputData.rightButton;
			break;
		}
		case MOUSE_BUTTON_RIGHT:
		{
			mouseButtonData = &s_mouseInputData.middleButton;
			break;
		}
		}
		if (mouseButtonData == nullptr) return false;
		Timestep outputDiff = CalculateTimestepDiff(mouseButtonData->lastTimeClicked);
		return mouseButtonData->buttonState == ACTION_PRESSED
			&& outputDiff.GetSeconds() >= MIN_MOUSE_HELD_TIME;
	}

	MathLib::Vector2 Input::GetMouseScreenPos()
	{
		return s_mouseInputData.screenPos;
	}

	MathLib::Vector2 Input::GetMouseScrollOffset()
	{
		Timestep diff = CalculateTimestepDiff(s_mouseInputData.scrollTime);
		if (diff > MAX_SCROLL_TIME)
		{
			return MathLib::Vector2::Zero;
		}
		return s_mouseInputData.scrollOffset;
	}

	bool Input::OnInputKeyEvent(InputKeyEvent& event)
	{
		if (event.keyCode == KEY_CODE_UNKNOWN)
		{
			return true;
		}

		auto found = s_keyCodes.find(event.keyCode);
		if (found != s_keyCodes.end())
		{
			InputKeyCodeData keyCodeData = found->second;
			// Calls the relased function.
			if (event.inputAction == ACTION_RELEASED)
			{
				InputKeyReleasedEvent inputKeyReleased(event.keyCode,
					CalculateTimestepDiff(keyCodeData.lastTimePressed));
				if (s_inputEventFunc != nullptr)
				{
					s_inputEventFunc(inputKeyReleased);
				}
				keyCodeData.isPressed = false;
				s_keyCodes[event.keyCode] = keyCodeData;
			}
			else if(event.inputAction == ACTION_PRESSED)
			{
				InputKeyPressedEvent inputKeyPressed(event.keyCode);
				if (s_inputEventFunc != nullptr)
				{
					s_inputEventFunc(inputKeyPressed);
				}
				keyCodeData.isPressed = true;
				keyCodeData.lastTimePressed = std::chrono::high_resolution_clock::now();
				s_keyCodes[event.keyCode] = keyCodeData;
			}
			return true;
		}
		InputKeyCodeData keyCodeData;
		keyCodeData.isPressed = event.inputAction == ACTION_PRESSED;
		if (keyCodeData.isPressed)
		{
			InputKeyPressedEvent inputKeyPressedEvent(event.keyCode);
			if (s_inputEventFunc != nullptr)
			{
				s_inputEventFunc(inputKeyPressedEvent);
			}
			keyCodeData.lastTimePressed =
				std::chrono::high_resolution_clock::now();
		}
		else
		{
			InputKeyReleasedEvent inputKeyReleasedEvent(event.keyCode, 0.0f);
			if (s_inputEventFunc != nullptr)
			{
				s_inputEventFunc(inputKeyReleasedEvent);
			}
		}
		s_keyCodes.emplace(event.keyCode, keyCodeData);
		return true;
	}
	
	bool Input::OnInputMouseButtonEvent(InputMouseButtonEvent& event)
	{
		InputMouseData::MouseButtonData* mouseButtonData = nullptr;
		switch (event.mouseButton)
		{
		case MOUSE_BUTTON_RIGHT: 
		{
			mouseButtonData = &s_mouseInputData.rightButton;
			break;
		}
		case MOUSE_BUTTON_LEFT:
		{
			mouseButtonData = &s_mouseInputData.leftButton;
			break;
		}
		case MOUSE_BUTTON_MIDDLE:
		{
			mouseButtonData = &s_mouseInputData.middleButton;
			break;
		}
		}
		if (mouseButtonData == nullptr)
		{
			return true;
		}
		mouseButtonData->buttonState = event.inputAction;
		if (mouseButtonData->buttonState == ACTION_PRESSED)
		{
			mouseButtonData->lastTimeClicked
				= std::chrono::high_resolution_clock::now();

			InputMouseButtonPressedEvent pressedEvent(event.mouseButton);
			if (s_inputEventFunc != nullptr)
			{
				s_inputEventFunc(pressedEvent);
			}
		}
		else if (mouseButtonData->buttonState == ACTION_RELEASED)
		{
			Timestep timeBetweenClicks =
				CalculateTimestepDiff(mouseButtonData->lastTimeClicked);
			InputMouseButtonReleasedEvent releasedEvent(event.mouseButton);
			if (s_inputEventFunc != nullptr)
			{
				s_inputEventFunc(releasedEvent);
			}
		}
		return true;
	}
	
	bool Input::OnInputMouseMoveEvent(InputMouseMoveEvent& event)
	{
		s_mouseInputData.screenPos = event.mousePos;
		return true;
	}

	bool Input::OnInputMouseScrollEvent(InputMouseScrollEvent& event)
	{
		s_mouseInputData.scrollOffset = event.scrollOffset;
		s_mouseInputData.scrollTime = std::chrono::high_resolution_clock::now();
		return true;
	}
}