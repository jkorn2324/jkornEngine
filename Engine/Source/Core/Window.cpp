#include "EnginePCH.h"
#include "Window.h"
#include "ApplicationEvent.h"

#include "GlfwWindow.h"
#include "Profiler.h"

namespace Engine
{
	WindowType Window::s_windowType = WindowType::GLFW_WINDOW;

	std::unique_ptr<Window> Window::GenerateWindow(const WindowProperties& properties)
	{
		PROFILE_SCOPE(GenerateWindow, Window);

		switch (s_windowType)
		{
		case WindowType::GLFW_WINDOW: return std::make_unique<GlfwWindow>(properties);
		}
		DebugAssert(false, "Failed to generate a window; unsupported type.");
		return nullptr;
	}
}
