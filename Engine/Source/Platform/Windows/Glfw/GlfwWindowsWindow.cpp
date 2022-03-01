#include "EnginePCH.h"
#include "GlfwWindowsWindow.h"

#include <unordered_map>
#include <Glfw\glfw3.h>
#include <Glfw\glfw3native.h>

#include <ApplicationEvent.h>

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "Input.h"
#include "PlatformInput.h"

namespace Engine
{

	// TODO: Static dict

#pragma region keycodeConverter


#pragma endregion

	static std::uint32_t s_numWindows = 0;

	GlfwWindowsWindow::GlfwWindowsWindow(const WindowProperties& properties)
		: Window(properties),
		m_window(nullptr),
		m_windowData(properties),
		m_dpiScale(1.0f)
	{
		Initialize();
	}

	GlfwWindowsWindow::~GlfwWindowsWindow()
	{
		Shutdown();
	}

	void GlfwWindowsWindow::SetCallback(const WindowEventCallback& callback)
	{
		m_windowData.callback = callback;
	}

	std::uint32_t GlfwWindowsWindow::GetWidth() const
	{
		return m_windowData.properties.width;
	}

	std::uint32_t GlfwWindowsWindow::GetHeight() const
	{
		return m_windowData.properties.height;
	}

	uint32_t GlfwWindowsWindow::GetMinHeight() const
	{
		return m_windowData.properties.minHeight;
	}

	uint32_t GlfwWindowsWindow::GetMinWidth() const
	{
		return m_windowData.properties.minWidth;
	}

	void GlfwWindowsWindow::SetMinSize(uint32_t w, uint32_t h)
	{
		w *= (uint32_t)m_dpiScale;
		h *= (uint32_t)m_dpiScale;

		glfwSetWindowSizeLimits(m_window, (int)w, (int)h, GLFW_DONT_CARE, GLFW_DONT_CARE);
		m_windowData.properties.minWidth = w;
		m_windowData.properties.minHeight = h;
	}

	float GlfwWindowsWindow::GetWindowDPIScale() const
	{
		return m_dpiScale;
	}

	void GlfwWindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		GraphicsRenderer::SwapBuffers();
	}

	void GlfwWindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
		s_numWindows--;

		if (s_numWindows <= 0)
		{
			glfwTerminate();
		}
	}

	bool GlfwWindowsWindow::IsValid() const
	{
		return s_numWindows > 0
			&& m_window != nullptr;
	}

	void GlfwWindowsWindow::SetVSync(bool vsync)
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

	bool GlfwWindowsWindow::IsVSync() const
	{
		return m_windowData.properties.vsync;
	}

	HWND GlfwWindowsWindow::GetHWND() const
	{
		return glfwGetWin32Window(m_window);
	}

	void GlfwWindowsWindow::Initialize()
	{
		if (s_numWindows <= 0)
		{
			// Sets no api so that we can use directx 11.
			int success = glfwInit();
			DebugAssert(success, "GLFW Failed to initialize.");
		}

		s_numWindows++;

		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			break;
		}
		}

		m_window = glfwCreateWindow((int)m_windowData.properties.width,
			(int)m_windowData.properties.height, m_windowData.properties.windowTitle.c_str(), nullptr, nullptr);
		DebugAssert(m_window != nullptr, "GLFW Failed to create the window.");
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowData);

		{
			float dpiScaleX, dpiScaleY;
			glfwGetWindowContentScale(m_window, &dpiScaleX, &dpiScaleY);
			m_dpiScale = dpiScaleX;
		}

		SetVSync(m_windowData.properties.vsync);
		SetMinSize(
			m_windowData.properties.minWidth, 
			m_windowData.properties.minHeight);

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
					windowData.minimized = windowData.properties.width == 0
						&& windowData.properties.height == 0;

					WindowResizedEvent event(windowData.properties.width,
						windowData.properties.height);

					if (windowData.callback != nullptr)
					{
						windowData.callback(event);
					}
				});
		}

		// Window keyboard event.
		{
			glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int modifiers)
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));

					InputKeyEvent inputKeyEvent(
						Input::GetPlatformInput().ToKeyCode(key),
						Input::GetPlatformInput().ToAction(action));
					if (windowData.callback != nullptr)
					{
						windowData.callback(inputKeyEvent);
					}
				});
		}

		// Mouse Input Callbacks.
		{
			glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int mouseButton,
				int action, int modifiers)
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));
					InputMouseButtonEvent mouseButtonEvent(
						Input::GetPlatformInput().ToAction(action),
						Input::GetPlatformInput().ToMouseButton(mouseButton));
					if (windowData.callback != nullptr)
					{
						windowData.callback(mouseButtonEvent);
					}
				});

			glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));
					InputMouseMoveEvent mouseMoveEvent(
						MathLib::Vector2((float)xPos, (float)yPos));
					if (windowData.callback != nullptr)
					{
						windowData.callback(mouseMoveEvent);
					}
				});

			glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));
					InputMouseScrollEvent mouseScrollEvent(
						MathLib::Vector2((float)xOffset, (float)yOffset));
					if (windowData.callback != nullptr)
					{
						windowData.callback(mouseScrollEvent);
					}
				});

			glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) -> void
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));
					
					bool newFocused = focused == GLFW_TRUE;
					if (newFocused != windowData.focused)
					{
						WindowFocusEvent event(windowData.focused);
						if (windowData.callback != nullptr)
						{
							windowData.callback(event);
						}
					}
					windowData.focused = newFocused;
				});
		}
	}
}