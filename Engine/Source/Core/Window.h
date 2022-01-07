#pragma once

#include <functional>
#include <memory>

#include "PlatformDetector.h"

struct GLFWwindow;

namespace Engine
{

	enum class WindowType
	{
		NONE,
		GLFW_WINDOWS_WINDOW,
		WIN32_WINDOWS_WINDOW
	};

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

	public:
		Window(const WindowProperties& params);
		virtual ~Window() { }

		virtual void SetCallback(const WindowEventCallback& callback)=0;

		virtual std::uint32_t GetWidth() const=0;
		virtual std::uint32_t GetHeight() const=0;

		virtual void OnUpdate()=0;
		virtual void Shutdown()=0;

		virtual bool IsValid() const=0;

		virtual void SetVSync(bool vsync)=0;
		virtual bool IsVSync() const=0;

	private:
		static WindowType s_windowType;

	public:
		static WindowType GetWindowType() { return s_windowType; }
		static std::unique_ptr<Window> GenerateWindow(const WindowProperties& properties);
	};
}