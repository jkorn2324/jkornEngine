#include "EnginePCH.h"
#include "Window.h"
#include "ApplicationEvent.h"

#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#include <iostream>

namespace Engine
{
	static std::uint32_t s_numWindows = 0;

	Window::Window(const WindowProperties& properties)
		: m_window(nullptr),
		m_windowData(properties)
	{
		Initialize();
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::SetCallback(const WindowEventCallback& callback)
	{
		m_windowData.callback = callback;
	}

	std::uint32_t Window::GetWidth() const
	{
		return m_windowData.properties.width;
	}

	std::uint32_t Window::GetHeight() const
	{
		return m_windowData.properties.height;
	}

	void Window::OnUpdate()
	{
		// Swaps the buffers according to graphics context.
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_window);
		s_numWindows--;

		if(s_numWindows <= 0)
		{
			glfwTerminate();
		}
	}

	bool Window::IsValid() const
	{
		return s_numWindows > 0
			&& m_window != nullptr;
	}

	void Window::SetVSync(bool vsync)
	{
		if (vsync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_windowData.properties.vsync = vsync;
	}

	bool Window::IsVSync() const
	{
		return m_windowData.properties.vsync;
	}

	HWND Window::GetHWND() const
	{
		return glfwGetWin32Window(m_window);
	}
	
	void Window::Initialize()
	{
		if (s_numWindows <= 0)
		{
			// Sets no api so that we can use directx 11.
			int success = glfwInit();
			DebugAssert(success, "GLFW Failed to initialize.");
		}

		s_numWindows++;

		// TODO: Do this per graphics api.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_window = glfwCreateWindow((int)m_windowData.properties.width,
			(int)m_windowData.properties.height, m_windowData.properties.windowTitle.c_str(), nullptr, nullptr);
		DebugAssert(m_window != nullptr, "GLFW Failed to create the window.");
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowData);
		
		SetVSync(m_windowData.properties.vsync);

		// Sets the window close callback function.
		{
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* closedWindow)
			{
				WindowData& windowData = *reinterpret_cast<WindowData*>(
					glfwGetWindowUserPointer(closedWindow));
				WindowClosedEvent event;

				// Invokes the callback function.
				if (windowData.callback != nullptr)
				{
					windowData.callback(event);
				}
			});
		}

		// The window resize event callback.
		{
			glfwSetWindowSizeCallback(m_window, [](GLFWwindow* resizedWindow, int width, int height)
			{
				WindowData& windowData = *reinterpret_cast<WindowData*>(
					glfwGetWindowUserPointer(resizedWindow));

				windowData.properties.width = (std::uint32_t)width;
				windowData.properties.height = (std::uint32_t)height;

				WindowResizedEvent event(windowData.properties.width, 
					windowData.properties.height);

				if (windowData.callback != nullptr)
				{
					windowData.callback(event);
				}
			});
		}
	}
}