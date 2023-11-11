#include "EnginePCH.h"
#include "PlatformImGuiLayer.h"

#include "GraphicsRenderer.h"
#include "MetalRenderingAPI.h"

#include "backends/imgui_impl_metal.h"
#include <imgui.h>

namespace Engine
{

// The platform imgui layer defined.
template<>
struct PlatformImGuiLayerDefined<RenderingAPIType, RenderingAPIType::METAL>
{
    static constexpr bool IsDefined = true;
};

template<>
void Platform::Internals::OnLayerAddedImpl<RenderingAPIType, RenderingAPIType::METAL>()
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    ImGui_ImplMetal_Init(renderingAPI.GetDevice());
}

template<>
void Platform::Internals::OnLayerRemovedImpl<RenderingAPIType, RenderingAPIType::METAL>()
{
    ImGui_ImplMetal_Shutdown();
}

template<>
void Platform::Internals::BeginFrameImpl<RenderingAPIType, RenderingAPIType::METAL>()
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    ImGui_ImplMetal_NewFrame(renderingAPI.GetRenderPassDescriptor());
}

template<>
void Platform::Internals::EndFrameImpl<RenderingAPIType, RenderingAPIType::METAL>()
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), renderingAPI.GetActiveCommandBuffer(), renderingAPI.GetRenderCommandEncoder());
}

}
