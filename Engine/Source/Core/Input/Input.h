#pragma once

#include "Event.h"
#include "EngineTime.h"

#include "Source\Vector.h"

#include <functional>

namespace Engine
{
	enum InputKeyCode
	{
		KEY_CODE_UNKNOWN,
		KEY_CODE_0,
		KEY_CODE_1,
		KEY_CODE_2,
		KEY_CODE_3,
		KEY_CODE_4,
		KEY_CODE_5,
		KEY_CODE_6,
		KEY_CODE_7,
		KEY_CODE_8,
		KEY_CODE_9,
		KEY_CODE_A,
		KEY_CODE_B,
		KEY_CODE_C,
		KEY_CODE_D,
		KEY_CODE_E,
		KEY_CODE_F,
		KEY_CODE_G,
		KEY_CODE_H,
		KEY_CODE_I,
		KEY_CODE_J,
		KEY_CODE_K,
		KEY_CODE_L,
		KEY_CODE_M,
		KEY_CODE_N,
		KEY_CODE_O,
		KEY_CODE_P,
		KEY_CODE_Q,
		KEY_CODE_R,
		KEY_CODE_S,
		KEY_CODE_T,
		KEY_CODE_U,
		KEY_CODE_V,
		KEY_CODE_W,
		KEY_CODE_X,
		KEY_CODE_Y,
		KEY_CODE_Z,
		KEY_CODE_SPACE,
		KEY_CODE_APOSTROPHE,
		KEY_CODE_COMMA,
		KEY_CODE_MINUS,
		KEY_CODE_PERIOD,
		KEY_CODE_SLASH,
		KEY_CODE_SEMICOLON,
		KEY_CODE_EQUAL,
		KEY_CODE_LEFT_BRACKET,
		KEY_CODE_BACKSLASH,
		KEY_CODE_RIGHT_BRACKET,
		KEY_CODE_GRAVE_ACCENT,
		KEY_CODE_WORLD_1,
		KEY_CODE_WORLD_2,
		KEY_CODE_ESCAPE,
		KEY_CODE_ENTER,
		KEY_CODE_TAB,
		KEY_CODE_BACKSPACE,
		KEY_CODE_INSERT,
		KEY_CODE_DELETE,
		KEY_CODE_RIGHT,
		KEY_CODE_LEFT,
		KEY_CODE_DOWN,
		KEY_CODE_UP,
		KEY_CODE_PAGE_UP,
		KEY_CODE_PAGE_DOWN,
		KEY_CODE_HOME,
		KEY_CODE_END,
		KEY_CODE_CAPS_LOCK,
		KEY_CODE_SCROLL_LOCK,
		KEY_CODE_NUM_LOCK,
		KEY_CODE_PRINT_SCREEN,
		KEY_CODE_PAUSE,
		KEY_CODE_F1,
		KEY_CODE_F2,
		KEY_CODE_F3,
		KEY_CODE_F4,
		KEY_CODE_F5,
		KEY_CODE_F6,
		KEY_CODE_F7,
		KEY_CODE_F8,
		KEY_CODE_F9,
		KEY_CODE_F10,
		KEY_CODE_F11,
		KEY_CODE_F12,
		KEY_CODE_F13,
		KEY_CODE_F14,
		KEY_CODE_F15,
		KEY_CODE_F16,
		KEY_CODE_F17,
		KEY_CODE_F18,
		KEY_CODE_F19,
		KEY_CODE_F20,
		KEY_CODE_F21,
		KEY_CODE_F22,
		KEY_CODE_F23,
		KEY_CODE_F24,
		KEY_CODE_F25,
		KEY_CODE_NUMPAD_0,
		KEY_CODE_NUMPAD_1,
		KEY_CODE_NUMPAD_2,
		KEY_CODE_NUMPAD_3,
		KEY_CODE_NUMPAD_4,
		KEY_CODE_NUMPAD_5,
		KEY_CODE_NUMPAD_6,
		KEY_CODE_NUMPAD_7,
		KEY_CODE_NUMPAD_8,
		KEY_CODE_NUMPAD_9,
		KEY_CODE_NUMPAD_DECIMAL,
		KEY_CODE_NUMPAD_DIVIDE,
		KEY_CODE_NUMPAD_MULTIPLY,
		KEY_CODE_NUMPAD_SUBTRACT,
		KEY_CODE_NUMPAD_ADD,
		KEY_CODE_NUMPAD_ENTER,
		KEY_CODE_NUMPAD_EQUAL,
		KEY_CODE_LEFT_SHIFT,
		KEY_CODE_LEFT_CONTROL,
		KEY_CODE_LEFT_ALT,
		KEY_CODE_LEFT_SUPER,
		KEY_CODE_RIGHT_SHIFT,
		KEY_CODE_RIGHT_CONTROL,
		KEY_CODE_RIGHT_ALT,
		KEY_CODE_RIGHT_SUPER
	};

	enum InputAction
	{
		ACTION_UNKNOWN,
		ACTION_PRESSED,
		ACTION_RELEASED
	};

	enum InputMouseButton
	{
		MOUSE_BUTTON_UNKNOWN,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE
	};

#pragma region key_events

	// Invoked when input key is either pressed or released.
	// Called from the window class only.
	class InputKeyEvent : public Event
	{
	public:
		explicit InputKeyEvent(InputKeyCode keyCode, InputAction action)
			: keyCode(keyCode), inputAction(action) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputKeyEvent)

		InputKeyCode keyCode;
		InputAction inputAction;
	};

	// Invoked only when the input key was pressed.
	// Called from the Input Manager class.
	class InputKeyPressedEvent : public Event
	{
	public:
		explicit InputKeyPressedEvent(InputKeyCode keycode)
			: keyCode(keycode) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputKeyPressedEvent)

		InputKeyCode keyCode;
	};

	// Invoked only when the input key was released,
	// called from the Input Manager class.
	class InputKeyReleasedEvent : public Event
	{
	public:
		explicit InputKeyReleasedEvent(InputKeyCode keycode, float timeHeld)
			: keyCode(keycode), timeKeyHeld(timeHeld) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputKeyReleasedEvent)

		Timestep timeKeyHeld;
		InputKeyCode keyCode;
	};

#pragma endregion

#pragma region mouse_events

	// Invoked from the window class, used to listen for mouse button events.
	class InputMouseButtonEvent : public Event
	{
	public:
		explicit InputMouseButtonEvent(InputAction inputAction, InputMouseButton button)
			: inputAction(inputAction), mouseButton(button) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputMouseButtonEvent)

		InputAction inputAction;
		InputMouseButton mouseButton;
	};

	// Invoked from input manager class.
	class InputMouseButtonPressedEvent : public Event
	{
	public:
		explicit InputMouseButtonPressedEvent(InputMouseButton button)
			: mouseButton(button) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputMouseButtonPressedEvent)

		InputMouseButton mouseButton;
	};

	// Invoked from input manager class.
	class InputMouseButtonReleasedEvent : public Event
	{
	public:
		explicit InputMouseButtonReleasedEvent(InputMouseButton button)
			: mouseButton(button) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputMouseButtonReleasedEvent)

		Timestep timeMouseHeld;
		InputMouseButton mouseButton;
	};

	class InputMouseMoveEvent : public Event
	{
	public:
		explicit InputMouseMoveEvent(const MathLib::Vector2& mousePos);

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputMouseMoveEvent)

		MathLib::Vector2 prevMousePos;
		MathLib::Vector2 mousePos;
	};

	class InputMouseScrollEvent : public Event
	{
	public:
		explicit InputMouseScrollEvent(const MathLib::Vector2& scrollOffset)
			: scrollOffset(scrollOffset) { }

		EVENT_CATEGORY_CLASS(Input)
		EVENT_TYPE_CLASS(InputMouseScrollEvent)

		MathLib::Vector2 scrollOffset;
	};

#pragma endregion

	class PlatformInput;

	class Input
	{
	public:
		using EventFunc = std::function<void(class Event&)>;

		static void Init();
		static void Release();

		static bool IsKeyPressed(InputKeyCode keyCode);
		static bool IsKeyPressed(InputKeyCode keyCode,
			const float maxTime);

		static bool IsKeyHeld(InputKeyCode keyCode);
		static float GetTimeKeyHeld(InputKeyCode keyCode);

		static bool IsMouseButtonPressed(InputMouseButton button);
		static bool IsMouseButtonPressed(InputMouseButton button,
			const float maxTime);
		static float GetMouseButtonTimeHeld(InputMouseButton button);

		static bool IsMouseButtonHeld(InputMouseButton button);
		static MathLib::Vector2 GetMouseScreenPos();
		static MathLib::Vector2 GetMouseScrollOffset();

		static PlatformInput& GetPlatformInput();

	private:
		static void OnEvent(Event& event);
		static void BindInputEventFunc(const EventFunc& func);
		
		static bool OnInputKeyEvent(InputKeyEvent& event);
		static bool OnInputMouseButtonEvent(InputMouseButtonEvent& event);
		static bool OnInputMouseMoveEvent(InputMouseMoveEvent& event);
		static bool OnInputMouseScrollEvent(InputMouseScrollEvent& event);

	private:
		friend class Application;
	};
}