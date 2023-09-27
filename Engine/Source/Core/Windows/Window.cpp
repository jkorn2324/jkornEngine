#include "EnginePCH.h"
#include "Window.h"
#include "ApplicationEvent.h"

#include "Profiler.h"

namespace Engine
{
	WindowType Window::s_windowType = WindowType::GLFW_WINDOW;

	std::unique_ptr<Window> Window::GenerateWindow(const WindowProperties& properties)
	{
		PROFILE_SCOPE(GenerateWindow, Window);
		return std::make_unique<Window>(properties);
	}
}
