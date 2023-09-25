#pragma once

#include "Window.h"
#include <Windows.h>

struct GLFWwindow;

namespace Engine
{


	class GlfwWindowsWindow : public Window
	{
	public:
		explicit GlfwWindowsWindow(const WindowProperties& properties);
		~GlfwWindowsWindow();

		 void SetMinSize(uint32_t w, uint32_t h) override;

		 void OnUpdate() override;
		 void Shutdown() override;

		 bool IsValid() const override;

		 void SetVSync(bool vsync) override;

		 HWND GetHWND() const;
		 const GLFWwindow* GetGlfwWindow() const { return m_window; }

	private:
		void Initialize();

	private:
		GLFWwindow* m_window;

		friend class GlfwImGuiLayer;
	};
}