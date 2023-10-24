#include "EnginePCH.h"
#include "MetalRenderingAPI.h"

#include "Window.h"
#include "PlatformWindowPtr.h"

#include <Metal/Metal.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

namespace Engine
{

MetalRenderingAPI::MetalRenderingAPI()
    : m_device(nullptr),
    m_swapChain(nullptr),
    m_commandQueue(nullptr),
    m_renderPassDescriptor(nullptr),
    m_clearColor(MathLib::Vector4::One),
    m_width(0), m_height(0)
{
    // TODO: Implementation
}

MetalRenderingAPI::~MetalRenderingAPI()
{
    // TODO: Implementation
}

bool MetalRenderingAPI::Initialize(Window *windowPtr)
{
    NSWindow* window = (NSWindow*)windowPtr->GetWindowPtr().GetDeviceWindowPtr();
    if (!window)
    {
        return false;
    }
    
    // Initialize the Device
    m_device = MTLCreateSystemDefaultDevice();
    if (!m_device)
    {
        JKORN_ENGINE_ASSERT(false, "Failed to Initialize device.");
        return false;
    }
    
    // Initialize the command buffer.
    m_commandQueue = [m_device newCommandQueue];
    if (!m_commandQueue)
    {
        JKORN_ENGINE_ASSERT(m_commandQueue, "Failed to initialize metal command queue.");
        return false;
    }
    
    // Initialize the swap chain (CA Metal Layer)
    {
        CAMetalLayerPtr swapChain = [CAMetalLayer layer];
        if (!swapChain)
        {
            JKORN_ENGINE_ASSERT(false, "Failed to initialize swap chain.")
            return false;
        }
        
        swapChain.device = m_device;
        swapChain.pixelFormat = MTLPixelFormatBGRA8Unorm;
        swapChain.bounds = window.contentView.bounds;
        window.contentView.layer = swapChain;
        window.contentView.wantsLayer = YES;
        m_swapChain = swapChain;
    }
    
    m_renderPassDescriptor = [MTLRenderPassDescriptor new];
    if (!m_renderPassDescriptor)
    {
        JKORN_ENGINE_ASSERT(false, "Failed to create the render pass descriptor.");
        return false;
    }
    // TODO: More Implementation, If Necessary
    return true;
}

void MetalRenderingAPI::SetViewport(float x, float y, float width, float height)
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::SetResolution(uint32_t width, uint32_t height)
{
    // TODO: Implementations
    m_width = width;
    m_height = height;
}

void MetalRenderingAPI::SetClearColor(const MathLib::Vector4 &color)
{
    m_clearColor = color;
}

void MetalRenderingAPI::SwapBuffers()
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::Clear()
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::Draw(VertexArray* vertexArray)
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::SetWireframe(bool wireframeMode)
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

bool MetalRenderingAPI::IsWireframe() const
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
    return false;
};

void MetalRenderingAPI::ClearTexture(uint32_t slot)
{
    // TODO: Implementation
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

uint32_t MetalRenderingAPI::GetWidth() const
{
    return m_width;
}

uint32_t MetalRenderingAPI::GetHeight() const
{
    return m_height;
}

}
