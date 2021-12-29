#pragma once

struct GLFWwindow;

namespace Engine
{

// Used so that we can expose the native win32 api for directx.

/* #ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif */

	class Window
	{
	public:
		Window(const char* windowTitle, std::uint32_t width,
			std::uint32_t height);
		~Window();

		std::uint32_t GetWidth() const;
		std::uint32_t GetHeight() const;

		void Shutdown();

		bool IsValid() const;

	private:
		void Initialize(const char* windowTitle);

	private:
		GLFWwindow* m_window;

		std::uint32_t m_width;
		std::uint32_t m_height;
	};
}