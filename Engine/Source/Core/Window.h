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
		GLFW_WINDOW,
		WIN32_WINDOWS_WINDOW
	};

	struct WindowProperties
	{
		std::string windowTitle;
		uint32_t width;
		uint32_t height;

		uint32_t minWidth;
		uint32_t minHeight;

		bool vsync = true;
		bool resizable = false;
	};

	class IEvent;

	class Window
	{
	public:
		using WindowEventCallback = std::function<void(IEvent&)>;

		// The Window Data for the Window
		struct WindowData
		{
			WindowProperties properties;
			WindowEventCallback callback;
			float dpiScale = 1.0f;
			bool minimized, focused;

			WindowData(const WindowProperties& props)
				: properties(props), callback(nullptr),
				minimized(false), focused(true), dpiScale(1.0f)
			{
			}
		};

	public:
		Window(const WindowProperties& params)
			: m_windowData(params)
		{

		}

		virtual ~Window() { }

		void SetCallback(const WindowEventCallback& callback)
		{
			m_windowData.callback = callback;
		}

		uint32_t GetWidth() const
		{
			return m_windowData.properties.width;
		}

		uint32_t GetHeight() const
		{
			return m_windowData.properties.height;
		}

		uint32_t GetMinWidth() const
		{
			return m_windowData.properties.minWidth;
		}

		uint32_t GetMinHeight() const
		{
			return m_windowData.properties.minHeight;
		}

		virtual void SetMinSize(uint32_t w, uint32_t h) =0;

		float GetWindowDPIScale() const
		{
			return m_windowData.dpiScale;
		}

		virtual void OnUpdate()=0;
		virtual void Shutdown()=0;

		bool IsMinimized() const
		{
			return m_windowData.minimized;
		}

		bool IsFocused() const
		{
			return m_windowData.focused;
		}

		virtual bool IsValid() const=0;

		virtual void SetVSync(bool vsync) = 0;

		bool IsVSync() const
		{
			return m_windowData.properties.vsync;
		}

	protected:
		WindowData m_windowData;

	private:
		static WindowType s_windowType;

	public:
		static WindowType GetWindowType() { return s_windowType; }
		static std::unique_ptr<Window> GenerateWindow(const WindowProperties& properties);
	};
}
