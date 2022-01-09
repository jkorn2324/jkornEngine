#include "EnginePCH.h"
#include "DirectX11ImGuiLayer.h"

#include "imgui.h"
#include "backends\imgui_impl_dx11.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

namespace Engine
{

	void DirectX11ImGuiLayer::OnLayerAdded()
	{
		DirectX11RenderingAPI& api = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		ImGui_ImplDX11_Init(api.m_device, api.m_deviceContext);
	}
	
	void DirectX11ImGuiLayer::OnShutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}
	
	void DirectX11ImGuiLayer::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}
	
	void DirectX11ImGuiLayer::EndFrame()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}