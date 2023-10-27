#include "Event.h"
#include "InputTypes.h"
#include "Vector.h"
#include "EngineTime.h"

namespace Engine
{

	enum class InputEventType
	{
		InputKeyEventType, InputKeyPressedEventType, InputKeyReleasedEventType,

		InputMouseButtonEventType, InputMouseButtonPressedEventType, InputMouseButtonReleasedEventType,

		InputMouseMoveEventType, InputMouseScrollEventType
	};

	// Invoked when input key is either pressed or released.
	// Called from the window class only.
	class InputKeyEvent : public Event<InputEventType>
	{
	public:
		explicit InputKeyEvent(InputKeyCode keyCode, InputAction action)
			: keyCode(keyCode), inputAction(action) { }

		EVENT_TYPE_CLASS(InputEventType, InputKeyEventType)

		InputKeyCode keyCode;
		InputAction inputAction;
	};

	// Invoked only when the input key was pressed.
	// Called from the Input Manager class.
	class InputKeyPressedEvent : public Event<InputEventType>
	{
	public:
		explicit InputKeyPressedEvent(InputKeyCode keycode)
			: keyCode(keycode) { }

		EVENT_TYPE_CLASS(InputEventType, InputKeyPressedEventType)

		InputKeyCode keyCode;
	};

	// Invoked only when the input key was released,
	// called from the Input Manager class.
	class InputKeyReleasedEvent : public Event<InputEventType>
	{
	public:
		explicit InputKeyReleasedEvent(InputKeyCode keycode, float timeHeld)
			: keyCode(keycode), timeKeyHeld(timeHeld) { }

		EVENT_TYPE_CLASS(InputEventType, InputKeyReleasedEventType)

		Timestep timeKeyHeld;
		InputKeyCode keyCode;
	};

	// Invoked from the window class, used to listen for mouse button events.
	class InputMouseButtonEvent : public Event<InputEventType>
	{
	public:
		explicit InputMouseButtonEvent(InputAction inputAction, InputMouseButton button)
			: inputAction(inputAction), mouseButton(button) { }

		EVENT_TYPE_CLASS(InputEventType, InputMouseButtonEventType)

		InputAction inputAction;
		InputMouseButton mouseButton;
	};

	// Invoked from input manager class.
	class InputMouseButtonPressedEvent : public Event<InputEventType>
	{
	public:
		explicit InputMouseButtonPressedEvent(InputMouseButton button)
			: mouseButton(button) { }

		EVENT_TYPE_CLASS(InputEventType, InputMouseButtonPressedEventType)

		InputMouseButton mouseButton;
	};

	// Invoked from input manager class.
	class InputMouseButtonReleasedEvent : public Event<InputEventType>
	{
	public:
		explicit InputMouseButtonReleasedEvent(InputMouseButton button)
			: mouseButton(button) { }

		EVENT_TYPE_CLASS(InputEventType, InputMouseButtonReleasedEventType)

		Timestep timeMouseHeld;
		InputMouseButton mouseButton;
	};

	class InputMouseMoveEvent : public Event<InputEventType>
	{
	public:
		explicit InputMouseMoveEvent(const MathLib::Vector2& mousePos);

		EVENT_TYPE_CLASS(InputEventType, InputMouseMoveEventType)

		MathLib::Vector2 prevMousePos;
		MathLib::Vector2 mousePos;
	};

	class InputMouseScrollEvent : public Event<InputEventType>
	{
	public:
		explicit InputMouseScrollEvent(const MathLib::Vector2& scrollOffset)
			: scrollOffset(scrollOffset) { }

		EVENT_TYPE_CLASS(InputEventType, InputMouseScrollEventType)

		MathLib::Vector2 scrollOffset;
	};

}