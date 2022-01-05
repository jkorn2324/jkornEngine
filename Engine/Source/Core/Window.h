#pragma once

#include <functional>
#include <Windows.h>

#include "PlatformDetector.h"

struct GLFWwindow;

namespace Engine
{

	struct WindowProperties
	{
		std::string windowTitle;
		std::uint32_t width;
		std::uint32_t height;

		bool vsync = true;
		bool resizable = false;
	};

	class Window
	{
	public:
		using WindowEventCallback = std::function<void(class Event&)>;

		struct WindowData
		{
			WindowProperties properties;
			WindowEventCallback callback;

			WindowData(const WindowProperties& props)
				: properties(props), callback(nullptr)
			{
			}
		};

	public:
		Window(const WindowProperties& params);
		~Window();

		void SetCallback(const WindowEventCallback& callback);

		std::uint32_t GetWidth() const;
		std::uint32_t GetHeight() const;

		void OnUpdate();
		void Shutdown();

		bool IsValid() const;

		void SetVSync(bool vsync);
		bool IsVSync() const;

#ifdef PLATFORM_WINDOWS
		HWND GetHWND() const;
#endif

	private:
		void Initialize();

	private:
		GLFWwindow* m_window;
		WindowData m_windowData;
	};
}