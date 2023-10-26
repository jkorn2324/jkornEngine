#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

namespace Engine
{

	// The platform imgui layer defined.
	template<>
	struct PlatformImGuiLayerDefined<RenderingAPIType, RenderingAPIType::DIRECTX11>
	{
		static constexpr bool IsDefined = true;
	};

	template<>
	void Platform::Internals::OnLayerAddedImpl<RenderingAPIType, RenderingAPIType::DIRECTX11>()
	{
		DirectX11RenderingAPI& api = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		ImGui_ImplDX11_Init(api.GetDevice(), api.GetDeviceContext());
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