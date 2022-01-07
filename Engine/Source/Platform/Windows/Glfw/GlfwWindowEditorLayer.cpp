#include "EnginePCH.h"
#include "PlatformEditorLayer.h"
#include "GlfwWindowEditorLayer.h"

#include <imgui.h>
#include <backends\imgui_impl_glfw.h>

#include "RenderingAPI.h"
#include "Application.h"
#include "GlfwWindowsWindow.h"

namespace Engine
{
	void GlfwWindowEditorLayer::OnLayerAdded()
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

	void GlfwWindowEditorLayer::OnShutdown()
	{
		ImGui_ImplGlfw_Shutdown();
	}

	void GlfwWindowEditorLayer::BeginFrame()
	{
		ImGui_ImplGlfw_NewFrame();
	}

	void GlfwWindowEditorLayer::EndFrame() {  }
}