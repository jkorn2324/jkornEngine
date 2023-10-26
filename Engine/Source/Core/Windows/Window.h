#pragma once

#include <functional>
#include <memory>
#include <string>

#include "PlatformWindowPtr.h"
#include "PlatformDetector.h"

struct GLFWwindow;

namespace Engine
{
	// Defines the platform window type.
	using PlatformWindowType = GLFWwindow;
	// The platform window raw pointer type.
	using PlatformWindowPtr = WindowPtr<PlatformWindowType>;

	class IEvent;

	// The Window Event Callback.
	using WindowEventCallback = std::function<void(IEvent&)>;

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

	/**
	 * A Class representing the window.  
	 */
	class Window
	{
	public:
		Window(const WindowProperties& params)
			: m_windowData(params),
			m_windowPtr(m_windowData)
		{

		}

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

		void SetMinSize(uint32_t w, uint32_t h) 
		{ 
			uint32_t dpiScale = (uint32_t)GetWindowDPIScale();
			w *= dpiScale;
			h *= dpiScale;
			m_windowPtr.SetMinSize(w, h); 
			m_windowData.properties.minWidth = w;
			m_windowData.properties.minHeight = h;
		}

		float GetWindowDPIScale() const { return m_windowData.dpiScale; }

		void OnUpdate() { m_windowPtr.OnUpdate(*this); }

		void Shutdown() { m_windowPtr.OnShutdown(*this); }

		bool IsMinimized() const { return m_windowData.minimized; }

		bool IsFocused() const { return m_windowData.focused; }

		bool IsValid() const { return m_windowPtr.IsValid(); }

		void SetVSync(bool vSync) 
		{ 
			m_windowPtr.SetVSync(vSync); 
			m_windowData.properties.vsync = vSync;
		}

		bool IsVSync() const
		{
			return m_windowData.properties.vsync;
		}
		
		/**
		 * Gets the pointer to the window. 
		 */
		template<typename TRawPtr = void>
		TRawPtr* GetRawWindowPtr() { return (TRawPtr*)m_windowPtr.m_ptr; }
		/**
		 * Gets the pointer to the window.
		 */
		template<typename TRawPtr = void>
		const TRawPtr* GetRawWindowPtr() const { return (const TRawPtr*)m_windowPtr.m_ptr; }

		/**
		 * Gets the pointer to the window. 
		 */
		const PlatformWindowPtr& GetWindowPtr() const { return m_windowPtr; }

	protected:
		WindowData m_windowData;
		PlatformWindowPtr m_windowPtr;

	private:
		static WindowType s_windowType;

	public:
		static WindowType GetWindowType() { return s_windowType; }
		static std::unique_ptr<Window> GenerateWindow(const WindowProperties& properties);
	};
}
