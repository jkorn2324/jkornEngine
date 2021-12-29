#include "EnginePCH.h"
#include "Window.h"

#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

namespace Engine
{
	static bool s_glfwInitialized = false;

	Window::Window(const char* windowTitle, std::uint32_t width, std::uint32_t height)
		: m_width(width),
		m_height(height),
		m_window(nullptr)
	{
		Initialize(windowTitle);
	}

	Window::~Window()
	{
		Shutdown();
	}

	std::uint32_t Window::GetWidth() const
	{
		return m_width;
	}

	std::uint32_t Window::GetHeight() const
	{
		return m_height;
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	bool Window::IsValid() const
	{
		return s_glfwInitialized
			&& m_window != nullptr;
	}
	
	void Window::Initialize(const char* windowTitle)
	{
		if (!s_glfwInitialized)
		{
			// Sets no api so that we can use directx 11.
			glfwInitHint(GLFW_CLIENT_API, GLFW_NO_API);
			int success = glfwInit();
			DebugAssert(success, "GLFW Failed to initialize.");
			s_glfwInitialized = true;
		}

		m_window = glfwCreateWindow((int)m_width,
			(int)m_height, windowTitle, nullptr, nullptr);
		DebugAssert(m_window != nullptr, "GLFW Failed to create the window.");
		glfwMakeContextCurrent(m_window);
	}
}