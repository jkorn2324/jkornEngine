#include "EnginePCH.h"
#include "DirectX11EditorLayer.h"

#include "imgui.h"
#include "backends\imgui_impl_dx11.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

namespace Engine
{

	void DirectX11EditorLayer::OnLayerAdded()
	{
		DirectX11RenderingAPI* api = (DirectX11RenderingAPI*)GraphicsRenderer::Get()->GetRenderingAPI();
		ImGui_ImplDX11_Init(api->m_device, api->m_deviceContext);
	}
	
	void DirectX11EditorLayer::OnShutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}
	
	void DirectX11EditorLayer::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}
	
	void DirectX11EditorLayer::EndFrame()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}