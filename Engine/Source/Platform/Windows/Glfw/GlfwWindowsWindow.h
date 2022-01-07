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

			WindowData(const WindowProperties& props)
				: properties(props), callback(nullptr)
			{
			}
		};

	public:
		explicit GlfwWindowsWindow(const WindowProperties& properties);
		~GlfwWindowsWindow();

		void SetCallback(const WindowEventCallback& callback) override;

		 std::uint32_t GetWidth() const override;
		 std::uint32_t GetHeight() const override;

		 void OnUpdate() override;
		 void Shutdown() override;

		 bool IsValid() const override;

		 void SetVSync(bool vsync) override;
		 bool IsVSync() const override;

		 HWND GetHWND() const;

	private:
		void Initialize();

	private:
		GLFWwindow* m_window;
		WindowData m_windowData;

		friend class GlfwImGuiLayer;
	};
}