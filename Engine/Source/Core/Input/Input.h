#pragma once

#include "EngineTime.h"
#include "InputTypes.h"

#include "Vector.h"

#include <functional>

namespace Engine
{
	class PlatformInput;
	class IEvent;

	class InputKeyEvent;
	class InputMouseButtonEvent;
	class InputMouseMoveEvent;
	class InputMouseScrollEvent;

	class Input
	{
	public:
		using EventFunc = std::function<void(IEvent&)>;

		static void Init();
		static void Release();

		static void SetEnabled(bool enabled);
		static bool IsEnabled();

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
		static void OnEvent(IEvent& event);
		static void BindInputEventFunc(const EventFunc& func);
		
		static bool OnInputKeyEvent(InputKeyEvent& event);
		static bool OnInputMouseButtonEvent(InputMouseButtonEvent& event);
		static bool OnInputMouseMoveEvent(InputMouseMoveEvent& event);
		static bool OnInputMouseScrollEvent(InputMouseScrollEvent& event);

	private:
		friend class Application;
	};
}
