#include "EnginePCH.h"
#include "Window.h"
#include "ApplicationEvent.h"

#include "GlfwWindowsWindow.h"

namespace Engine
{
	WindowType Window::s_windowType = WindowType::GLFW_WINDOWS_WINDOW;

	std::unique_ptr<Window> Window::GenerateWindow(const WindowProperties& properties)
	{
		switch (s_windowType)
		{
		case WindowType::GLFW_WINDOWS_WINDOW: return std::make_unique<GlfwWindowsWindow>(properties);
		}
		DebugAssert(false, "Failed to generate a window; unsupported type.");
		return nullptr;
	}

	Window::Window(const WindowProperties& params) { }
}