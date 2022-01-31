#pragma once

#include "Input.h"

namespace Engine
{

	class PlatformInput
	{
	public:
		explicit PlatformInput() = default;
		virtual ~PlatformInput() = default;

		virtual InputKeyCode ToKeyCode(int platformKeyCode) const = 0;
		virtual int FromKeyCode(const InputKeyCode& keyCode) const = 0;

		virtual InputMouseButton ToMouseButton(int platformMouseCode) const = 0;
		virtual int FromMouseButton(const InputMouseButton& mouseCode) const = 0;

		virtual InputAction ToAction(int action) const=0;
		virtual int FromAction(const InputAction& mouseAction) const=0;

	private:
		static PlatformInput* CreatePlatformInput();

		friend class Input;
	};
}