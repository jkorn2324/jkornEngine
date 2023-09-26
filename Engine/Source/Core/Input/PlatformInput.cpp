#include "EnginePCH.h"
#include "PlatformInput.h"

#include "Window.h"

// TODO: MACROIFY THIS
#include "GlfwPlatformInput.h"

namespace Engine
{
	PlatformInput* PlatformInput::CreatePlatformInput()
	{
		switch (Window::GetWindowType())
		{
		case WindowType::GLFW_WINDOW: return new GlfwPlatformInput();
		}
		return nullptr;
	}
}
