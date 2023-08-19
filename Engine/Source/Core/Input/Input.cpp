#include "EnginePCH.h"
#include "Input.h"
#include "PlatformInput.h"

#include "Window.h"
#include "InputEvent.h"
#include "InputTypes.h"

#include <chrono>

namespace Engine
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;

	static const float MIN_KEY_HELD_TIME = 1.0f / 30.0f;
	static const float MIN_MOUSE_HELD_TIME = 1.0f / 50.0f;
	static const float MAX_SCROLL_TIME = 1.0f / 30.0f;



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
	static bool s_inputEnabled = true;

	Input::EventFunc s_inputEventFunc = nullptr;
	PlatformInput* s_platformInput = nullptr;

	static InputMouseData::MouseButtonData* GetInputMouseButtonData(InputMouseButton mouseButton)
	{
		InputMouseData::MouseButtonData* mouseData = nullptr;
		switch (mouseButton)
		{
		case Engine::MOUSE_BUTTON_LEFT:
			mouseData = &s_mouseInputData.leftButton;
			break;
		case Engine::MOUSE_BUTTON_RIGHT:
			mouseData = &s_mouseInputData.rightButton;
			break;
		case Engine::MOUSE_BUTTON_MIDDLE:
			mouseData = &s_mouseInputData.middleButton;
			break;
		}
		return mouseData;
	}

	void Input::Init()
	{
		s_platformInput = PlatformInput::CreatePlatformInput();
	}

	void Input::Release()
	{
		delete s_platformInput;
	}

	void Input::SetEnabled(bool enabled)
	{
		s_inputEnabled = enabled;
	}

	bool Input::IsEnabled()
	{
		return s_inputEnabled;
	}

	void Input::BindInputEventFunc(const Input::EventFunc& func)
	{
		s_inputEventFunc = func;
	}

	void Input::OnEvent(IEvent& event)
	{
		EventDispatcher eventDispatcher(event);
		eventDispatcher.Invoke<InputEventType, InputKeyEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputKeyEvent));
		eventDispatcher.Invoke<InputEventType, InputMouseButtonEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseButtonEvent));
		eventDispatcher.Invoke<InputEventType, InputMouseMoveEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseMoveEvent));
		eventDispatcher.Invoke<InputEventType, InputMouseScrollEvent>(
			BIND_STATIC_EVENT_FUNCTION(Input::OnInputMouseScrollEvent));
	}

	bool Input::IsKeyPressed(InputKeyCode keyCode)
	{
		return IsKeyPressed(keyCode, MIN_KEY_HELD_TIME);
	}

	bool Input::IsKeyPressed(InputKeyCode keyCode, const float maxTime)
	{
		if (!s_inputEnabled) return false;

		const auto& found = s_keyCodes.find(keyCode);
		if (found != s_keyCodes.end()
			&& found->second.isPressed)
		{
			return GetTimeKeyHeld(keyCode) < maxTime;
		}
		return false;
	}
	
	bool Input::IsKeyHeld(InputKeyCode keyCode)
	{
		return s_inputEnabled && GetTimeKeyHeld(keyCode) >= MIN_KEY_HELD_TIME;
	}

	float Input::GetTimeKeyHeld(InputKeyCode keyCode)
	{
		if (!s_inputEnabled) return 0.0f;

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
		return s_inputEnabled && IsMouseButtonPressed(button, MIN_MOUSE_HELD_TIME);
	}

	bool Input::IsMouseButtonPressed(InputMouseButton button, const float maxPressedTime)
	{
		if (!s_inputEnabled) return false;

		InputMouseData::MouseButtonData* mouseButtonData = GetInputMouseButtonData(button);
		if (mouseButtonData == nullptr) return false;

		Timestep outputDiff = CalculateTimestepDiff(mouseButtonData->lastTimeClicked);
		return mouseButtonData->buttonState == ACTION_PRESSED
			&& outputDiff.GetRawSeconds() < maxPressedTime;
	}


	bool Input::IsMouseButtonHeld(InputMouseButton button)
	{
		return s_inputEnabled && GetMouseButtonTimeHeld(button) >= MIN_MOUSE_HELD_TIME;
	}

	float Input::GetMouseButtonTimeHeld(InputMouseButton button)
	{
		if (!s_inputEnabled) return 0.0f;
		InputMouseData::MouseButtonData* mouseButtonData = GetInputMouseButtonData(button);
		if (mouseButtonData == nullptr) return false;
		Timestep outDiff = CalculateTimestepDiff(mouseButtonData->lastTimeClicked);
		if (mouseButtonData->buttonState != ACTION_PRESSED)
		{
			return 0.0f;
		}
		return outDiff.GetRawSeconds();
	}

	MathLib::Vector2 Input::GetMouseScreenPos()
	{
		return s_mouseInputData.screenPos;
	}

	MathLib::Vector2 Input::GetMouseScrollOffset()
	{
		if (!s_inputEnabled) return MathLib::Vector2::Zero;
		Timestep diff = CalculateTimestepDiff(s_mouseInputData.scrollTime);
		if (diff > MAX_SCROLL_TIME)
		{
			return MathLib::Vector2::Zero;
		}
		return s_mouseInputData.scrollOffset;
	}

	PlatformInput& Input::GetPlatformInput()
	{
		DebugAssert(s_platformInput != nullptr, "Platform input exists.");
		return *s_platformInput;
	}

	bool Input::OnInputKeyEvent(InputKeyEvent& event)
	{
		if (!s_inputEnabled || event.keyCode == KEY_CODE_UNKNOWN)
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
		InputMouseData::MouseButtonData* mouseButtonData = GetInputMouseButtonData(event.mouseButton);
		if (!s_inputEnabled || mouseButtonData == nullptr) return true;
		
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