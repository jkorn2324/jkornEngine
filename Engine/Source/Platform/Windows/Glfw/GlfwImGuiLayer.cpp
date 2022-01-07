#include "EnginePCH.h"
#include "GlfwImGuiLayer.h"

#include <imgui.h>
#include <backends\imgui_impl_glfw.h>
#include <Glfw\glfw3.h>

#include "RenderingAPI.h"
#include "Application.h"
#include "GlfwWindowsWindow.h"


namespace Engine
{
	void GlfwImGuiLayer::OnLayerAdded()
	{
		GlfwWindowsWindow* window = (GlfwWindowsWindow*)(&Application::Get().GetWindow());
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				ImGui_ImplGlfw_InitForOther(window->m_window, true);
				break;
			}
		}
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
			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
	}
}