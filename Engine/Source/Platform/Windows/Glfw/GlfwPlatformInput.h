#pragma once

#include "PlatformInput.h"

namespace Engine
{


	class GlfwPlatformInput : public PlatformInput
	{
	public:
		explicit GlfwPlatformInput();
		~GlfwPlatformInput() = default;

		InputKeyCode ToKeyCode(int platformKeyCode) const override;
		int FromKeyCode(const InputKeyCode& keyCode) const override;

		InputMouseButton ToMouseButton(int platformMouseCode) const override;
		int FromMouseButton(const InputMouseButton& mouseButton) const override;

		InputAction ToAction(int platformMouseAction) const override;
		int FromAction(const InputAction& mouseAction) const override;
	};
}