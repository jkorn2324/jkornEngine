#include "EnginePCH.h"
#include "MetalImGuiLayer.h"

#include "GraphicsRenderer.h"
#include "MetalRenderingAPI.h"

#include "backends/imgui_impl_metal.h"
#include <imgui.h>

namespace Engine
{

void MetalImGuiLayer::OnLayerAdded()
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    ImGui_ImplMetal_Init(renderingAPI.m_device);
}

void MetalImGuiLayer::OnShutdown()
{
    ImGui_ImplMetal_Shutdown();
}

static id<MTLRenderCommandEncoder> s_commandEncoder;

void MetalImGuiLayer::BeginFrame()
{
    // TODO: Implementation
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    ImGui_ImplMetal_NewFrame(renderingAPI.m_renderPassDescriptor);
}

void MetalImGuiLayer::EndFrame()
{
    // TODO: Implementation
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    // ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), renderingAPI.m_commandQueue);
}

}
