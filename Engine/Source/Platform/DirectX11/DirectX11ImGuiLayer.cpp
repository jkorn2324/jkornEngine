#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>

namespace Engine
{

	template<>
	constexpr bool Platform::Internals::IsDefined<RenderingAPIType, RenderingAPIType::DIRECTX11>() { return true; }

	template<>
	void Platform::Internals::OnLayerAddedImpl<RenderingAPIType, RenderingAPIType::DIRECTX11>()
	{
		DirectX11RenderingAPI& api = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		ImGui_ImplDX11_Init(api.m_device, api.m_deviceContext);
	}

	template<>
	void Platform::Internals::OnLayerRemovedImpl<RenderingAPIType, RenderingAPIType::DIRECTX11>()
	{
		ImGui_ImplDX11_Shutdown();
	}

	template<>
	void Platform::Internals::BeginFrameImpl<RenderingAPIType, RenderingAPIType::DIRECTX11>()
	{
		ImGui_ImplDX11_NewFrame();
	}

	template<>
	void Platform::Internals::EndFrameImpl<RenderingAPIType, RenderingAPIType::DIRECTX11>()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}