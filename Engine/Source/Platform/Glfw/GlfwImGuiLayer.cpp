#include "EnginePCH.h"
#include "GlfwImGuiLayer.h"

#include <imgui.h>
#include <backends\imgui_impl_glfw.h>
#include <Glfw\glfw3.h>

#include "RenderingAPI.h"
#include "Application.h"
#include "Window.h"

namespace Engine
{
	void GlfwImGuiLayer::OnLayerAdded()
	{
		Window& window = Application::Get().GetWindow();
#if defined(GRAPHICS_API_DIRECTX11)
		ImGui_ImplGlfw_InitForOther(window.GetRawWindowPtr<GLFWwindow>(), true);
#endif
	}

	void GlfwImGuiLayer::OnShutdown()
	{
		ImGui_ImplGlfw_Shutdown();
	}

	void GlfwImGuiLayer::BeginFrame()
	{
		ImGui_ImplGlfw_NewFrame();
	}

	void GlfwImGuiLayer::EndFrame() 
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Window& window = Application::Get().GetWindow();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window.GetRawWindowPtr<GLFWwindow>());
		}
	}
}
