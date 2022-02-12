#pragma once

#include "Window.h"
#include <Windows.h>

struct GLFWwindow;

namespace Engine
{


	class GlfwWindowsWindow : public Window
	{
	public:
		struct WindowData
		{
			WindowProperties properties;
			WindowEventCallback callback;
			bool minimized, focused;

			WindowData(const WindowProperties& props)
				: properties(props), callback(nullptr), 
				minimized(false), focused(true)
			{
			}
		};

	public:
		explicit GlfwWindowsWindow(const WindowProperties& properties);
		~GlfwWindowsWindow();

		void SetCallback(const WindowEventCallback& callback) override;

		 std::uint32_t GetWidth() const override;
		 std::uint32_t GetHeight() const override;

		 uint32_t GetMinWidth() const override;
		 uint32_t GetMinHeight() const override;

		 void SetMinSize(uint32_t w, uint32_t h) override;

		 float GetWindowDPIScale() const override;

		 void OnUpdate() override;
		 void Shutdown() override;

		 bool IsValid() const override;
		 bool IsMinimized() const override { return m_windowData.minimized; }
		 bool IsFocused() const override { return m_windowData.focused; }

		 void SetVSync(bool vsync) override;
		 bool IsVSync() const override;

		 HWND GetHWND() const;
		 const GLFWwindow* GetGlfwWindow() const { return m_window; }

	private:
		void Initialize();

	private:
		GLFWwindow* m_window;
		WindowData m_windowData;

		uint32_t m_minWidth, m_minHeight;
		float m_dpiScale;

		friend class GlfwImGuiLayer;
	};
}