#include "EnginePCH.h"

#include "PlatformWindowPtr.h"

#include "Window.h"
#include "Profiler.h"
#include "ApplicationEvent.h"
#include "GraphicsRenderer.h"
#include "RenderingAPI.h"
#include "Input.h"
#include "InputEvent.h"
#include "PlatformInput.h"

#include <Glfw/glfw3.h>

#if defined(__APPLE__)
#ifndef GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#endif
#include <Glfw/glfw3native.h>

namespace Engine::Platform::Internals
{
	static uint32_t s_numWindows = 0;

	// Creates a pointer.
	template<>
	void AllocWindowPtr(WindowPtr<GLFWwindow>& ptr, WindowData& windowData)
	{
		PROFILE_SCOPE(Initialize, GlfwWindowPtr);

		if (s_numWindows <= 0)
		{
			// Sets no api so that we can use directx 11.
			int success = glfwInit();
            JKORN_ENGINE_ASSERT(success, "GLFW Failed to initialize.");
		}

		s_numWindows++;

#if defined(GRAPHICS_API_DIRECTX11)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
#if defined(GRAPHICS_API_METAL)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		WindowProperties& properties = windowData.properties;
		ptr.m_ptr = glfwCreateWindow((int)properties.width,
			(int)properties.height, properties.windowTitle.c_str(), nullptr, nullptr);
        JKORN_ENGINE_ASSERT(ptr.m_ptr != nullptr, "GLFW Failed to create the window.");
		glfwMakeContextCurrent(ptr.m_ptr);
		glfwSetWindowUserPointer(ptr.m_ptr, &windowData);

		{
			float dpiScaleX, dpiScaleY;
			glfwGetWindowContentScale(ptr.m_ptr, &dpiScaleX, &dpiScaleY);
			windowData.dpiScale = dpiScaleX;
		}

		ptr.SetVSync(properties.vsync);
		ptr.SetMinSize(properties.minWidth, properties.minHeight);

		// Sets the window close callback function.
		{
			glfwSetWindowCloseCallback(ptr.m_ptr, [](GLFWwindow* closedWindow)
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
			glfwSetWindowSizeCallback(ptr.m_ptr, [](GLFWwindow* resizedWindow, int width, int height)
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
			glfwSetKeyCallback(ptr.m_ptr, [](GLFWwindow* window, int key, int scancode, int action, int modifiers)
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
			glfwSetMouseButtonCallback(ptr.m_ptr, [](GLFWwindow* window, int mouseButton, int action, int modifiers)
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

			glfwSetCursorPosCallback(ptr.m_ptr, [](GLFWwindow* window, double xPos, double yPos)
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

			glfwSetScrollCallback(ptr.m_ptr, [](GLFWwindow* window, double xOffset, double yOffset)
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

			glfwSetWindowFocusCallback(ptr.m_ptr, [](GLFWwindow* window, int focused) -> void
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

	// Sets the min size for the glfw window.
	template<>
	void SetMinSize(WindowPtr<GLFWwindow>& windowPtr, uint32_t w, uint32_t h)
	{
		glfwSetWindowSizeLimits(windowPtr.m_ptr, (int)w, (int)h, GLFW_DONT_CARE, GLFW_DONT_CARE);
	}

	// Called to update the window pointer.
	template<>
	void OnUpdate(WindowPtr<GLFWwindow>& windowPtr, Window& window)
	{
		PROFILE_SCOPE(OnUpdate, WindowPtr<GLFWwindow>);

		glfwPollEvents();
		GraphicsRenderer::Present();
	}

	// Called to shut down the window.
	template<>
	void OnShutdown(WindowPtr<GLFWwindow>& windowPtr, Window& window)
	{
		glfwDestroyWindow(windowPtr.m_ptr);
		s_numWindows--;

		if (s_numWindows <= 0)
		{
			glfwTerminate();
		}
		windowPtr.m_ptr = nullptr;
	}

	// Called to determines whether or not a pointer is valid.
	template<>
	bool IsValid(const WindowPtr<GLFWwindow>& windowPtr)
	{
		return s_numWindows > 0 && windowPtr.m_ptr != nullptr;
	}

	// Sets vsync.
	template<>
	void SetVSync(WindowPtr<GLFWwindow>& windowPtr, const bool vSync)
	{
		glfwSwapInterval((int)vSync);
	}

    template<>
    void* GetDeviceWindowPtr(const WindowPtr<GLFWwindow>& windowPtr)
    {
#if defined(PLATFORM_WINDOWS)
        return (void*)glfwGetWin32Window(windowPtr.m_ptr);
#elif defined(PLATFORM_MACOS)
        return (void*)glfwGetCocoaWindow(windowPtr.m_ptr);
#else
        return nullptr;
#endif
    }
}
