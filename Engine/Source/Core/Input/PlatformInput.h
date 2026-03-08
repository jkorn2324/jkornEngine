#pragma once

#include "Input.h"
#include <imgui.h>

namespace Engine
{

	class PlatformInput
	{
	public:
		explicit PlatformInput() = default;
		virtual ~PlatformInput() = default;

		virtual InputKeyCode ToKeyCode(int platformKeyCode) const = 0;
		virtual int FromKeyCode(const InputKeyCode keyCode) const = 0;

		template<typename TKeyCodeType>
		TKeyCodeType FromKeyCode(const InputKeyCode keyCode) const
		{
			int32_t outputKey = FromKeyCode(keyCode);
			return *(reinterpret_cast<TKeyCodeType*>(&outputKey));
		}

		virtual InputMouseButton ToMouseButton(int platformMouseCode) const = 0;
		virtual int FromMouseButton(const InputMouseButton mouseCode) const = 0;

		template<typename TMouseButton>
		TMouseButton FromMouseButton(const InputMouseButton mouseCode) const
		{
			int32_t outputMouseCode = FromMouseButton(mouseCode);
			return *(reinterpret_cast<TMouseButton*>(&outputMouseCode));
		}

		virtual InputAction ToAction(int action) const = 0;
		virtual int FromAction(const InputAction mouseAction) const = 0;

		template<typename TAction>
		TAction FromAction(const InputAction mouseAction) const
		{
			int32_t outputAction = FromAction(mouseAction);
			return *(reinterpret_cast<TAction*>(&outputAction));
		}

		/**
		 * This converts the platform key code to an im gui key.
		 */
		virtual ImGuiKey PlatformKeyToImGuiKey(int platformKeyCode) const = 0;

		/**
		 * This converts the key code to the imgui key.
		 */
		ImGuiKey KeyCodeToImGuiKey(const InputKeyCode inKeyCode) const
		{
			const int platformKeyCode = FromKeyCode(inKeyCode);
			return PlatformKeyToImGuiKey(platformKeyCode);
		}

	private:
		static PlatformInput* CreatePlatformInput();

		friend class Input;
	};
}