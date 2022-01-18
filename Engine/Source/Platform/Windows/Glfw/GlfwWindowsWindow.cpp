#include "EnginePCH.h"
#include "GlfwWindowsWindow.h"

#include <unordered_map>
#include <Glfw\glfw3.h>
#include <Glfw\glfw3native.h>

#include <ApplicationEvent.h>

#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "Input.h"

namespace Engine
{

	// TODO: Static dict

#pragma region keycodeConverter

	static InputKeyCode s_keyCodes[GLFW_KEY_MENU];
	static bool s_keyCodesInitialized = false;

	static InputMouseButton ConvertGlfwMouseButton(int mouseCode)
	{
		switch (mouseCode)
		{
		case GLFW_MOUSE_BUTTON_1: return MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_2: return MOUSE_BUTTON_RIGHT;
		case GLFW_MOUSE_BUTTON_MIDDLE: return MOUSE_BUTTON_MIDDLE;
		}
		return MOUSE_BUTTON_UNKNOWN;
	}

	static InputKeyCode ConvertGlfwKeyCodeDynamic(int keyCode)
	{


		// Covers 0 - 9
		if (keyCode >= GLFW_KEY_0
			&& keyCode <= GLFW_KEY_9)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_0) + KEY_CODE_0);
		}
		// Covers A - Z
		if (keyCode >= GLFW_KEY_A
			&& keyCode <= GLFW_KEY_Z)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_A) + KEY_CODE_A);
		}

		// Covers Space - Slash
		if (keyCode >= GLFW_KEY_SPACE
			&& keyCode <= GLFW_KEY_SLASH)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_SPACE) + KEY_CODE_SPACE);
		}

		if (keyCode == GLFW_KEY_SEMICOLON
			|| keyCode == GLFW_KEY_EQUAL)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_SEMICOLON) + KEY_CODE_SEMICOLON);
		}

		if (keyCode >= GLFW_KEY_LEFT_BRACKET
			&& keyCode <= GLFW_KEY_RIGHT_BRACKET)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_LEFT_BRACKET) + KEY_CODE_LEFT_BRACKET);
		}

		if (keyCode == GLFW_KEY_GRAVE_ACCENT)
		{
			return KEY_CODE_GRAVE_ACCENT;
		}

		if (keyCode == GLFW_KEY_WORLD_1
			|| keyCode == GLFW_KEY_WORLD_2)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_WORLD_1) + KEY_CODE_WORLD_1);
		}

		if (keyCode >= GLFW_KEY_ESCAPE
			&& keyCode <= GLFW_KEY_END)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_ESCAPE) + KEY_CODE_ESCAPE);
		}

		if (keyCode >= GLFW_KEY_CAPS_LOCK
			&& keyCode <= GLFW_KEY_PAUSE)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_CAPS_LOCK) + KEY_CODE_CAPS_LOCK);
		}

		if (keyCode >= GLFW_KEY_F1
			&& keyCode <= GLFW_KEY_F25)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_F1) + KEY_CODE_F1);
		}

		if (keyCode >= GLFW_KEY_KP_0
			&& keyCode <= GLFW_KEY_KP_EQUAL)
		{
			return (InputKeyCode)((keyCode - GLFW_KEY_KP_0) + KEY_CODE_NUMPAD_0);
		}
		return (InputKeyCode)((keyCode - GLFW_KEY_LEFT_SHIFT) + KEY_CODE_LEFT_SHIFT);
	}

	static InputKeyCode ConvertGlfwKeyCode(int keyCode)
	{
		if (keyCode == GLFW_KEY_UNKNOWN)
		{
			return KEY_CODE_UNKNOWN;
		}

		if (s_keyCodes[keyCode] != KEY_CODE_UNKNOWN)
		{
			return s_keyCodes[keyCode];
		}
		InputKeyCode code = ConvertGlfwKeyCodeDynamic(keyCode);
		s_keyCodes[keyCode] = code;
		return code;
	}

	static InputAction ConvertGlfwInputAction(int inputAction)
	{
		switch (inputAction)
		{
		case GLFW_PRESS: return ACTION_PRESSED;
		case GLFW_RELEASE: return ACTION_RELEASED;
		}
		return ACTION_UNKNOWN;
	}

#pragma endregion

	static std::uint32_t s_numWindows = 0;

	GlfwWindowsWindow::GlfwWindowsWindow(const WindowProperties& properties)
		: Window(properties),
		m_window(nullptr),
		m_windowData(properties)
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

		// TODO: Use macros here
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

		// Window keyboard event.
		{
			glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int modifiers)
				{
					WindowData& windowData = *reinterpret_cast<WindowData*>(
						glfwGetWindowUserPointer(window));

					InputKeyEvent inputKeyEvent(ConvertGlfwKeyCode(key),
						ConvertGlfwInputAction(action));
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
					InputMouseButtonEvent mouseButtonEvent(ConvertGlfwInputAction(action), 
						ConvertGlfwMouseButton(mouseButton));
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
		}
	}
}