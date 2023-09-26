#pragma once

#include "Window.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

struct GLFWwindow;

namespace Engine
{


	class GlfwWindow : public Window
	{
	public:
		explicit GlfwWindow(const WindowProperties& properties);
		~GlfwWindow();

		 void SetMinSize(uint32_t w, uint32_t h) override;

		 void OnUpdate() override;
		 void Shutdown() override;

		 bool IsValid() const override;

		 void SetVSync(bool vsync) override;

#ifdef PLATFORM_WINDOWS
		 HWND GetHWND() const;
#endif
		 const GLFWwindow* GetGlfwWindow() const { return m_window; }

	private:
		void Initialize();

	private:
		GLFWwindow* m_window;

		friend class GlfwImGuiLayer;
	};
}
