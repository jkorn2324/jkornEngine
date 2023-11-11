#include "EnginePCH.h"
#include "MetalRenderingAPI.h"

#include "Window.h"
#include "PlatformWindowPtr.h"

#include <Metal/Metal.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#include "MetalVertexBuffer.h"
#include "MetalIndexBuffer.h"

namespace Engine
{

MetalRenderingAPI::MetalRenderingAPI()
    : m_device(nullptr),
    m_swapChain(nullptr),
    m_commandQueue(nullptr),
    m_activeCommandBuffer(nullptr),
    m_renderCommandEncoder(nullptr),
    m_renderPassDescriptor(nullptr),
    m_clearColor(MathLib::Vector4::One),
    m_width(0), m_height(0)
{
    // TODO: Implementation
}

MetalRenderingAPI::~MetalRenderingAPI()
{
    // TODO: Implementation
    [m_swapChain release];
    [m_commandQueue release];

    m_commandQueue = nullptr;
    m_swapChain = nullptr;
    m_device = nullptr;
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
    
    // Initialize the Metal Layer (default metal layer)
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
    // TODO: More Implementation, If Necessary
    return true;
}

void MetalRenderingAPI::SetViewport(float x, float y, float width, float height)
{
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
    // TODO: Implementation
}

void MetalRenderingAPI::SetResolution(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    
#if false
    // Sets the drawable size for the swap chain.
    if (m_swapChain)
    {
        m_swapChain.drawableSize = CGSizeMake(width, height);
    }
#endif
}

void MetalRenderingAPI::SetClearColor(const MathLib::Vector4 &color)
{
    m_clearColor = color;
}


void MetalRenderingAPI::OnBeginRender(MTLRenderPassDescriptorPtr descriptor)
{
    // The current render pass descriptor.
    m_renderPassDescriptor = descriptor;
    
    // Applies the clear color to the metal rendering descriptor.
    {
        size_t length = (size_t)descriptor.renderTargetArrayLength;
        for (size_t index = 0; index < length; ++index)
        {
            MTLRenderPassColorAttachmentDescriptor* desc = descriptor.colorAttachments[index];
            desc.clearColor = MTLClearColorMake(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        }
    }

    m_activeCommandBuffer = [m_commandQueue commandBuffer];
    m_renderCommandEncoder = [m_activeCommandBuffer renderCommandEncoderWithDescriptor:descriptor];
}

void MetalRenderingAPI::OnEndRender()
{
    // Called to stop encoding.
    [m_renderCommandEncoder endEncoding];
    [m_activeCommandBuffer commit];
}

void MetalRenderingAPI::Present()
{
    JKORN_ENGINE_ASSERT(m_activeCommandBuffer, "The command buffer must exist for metal.");
    
    id<MTLDrawable> drawable = [m_swapChain nextDrawable];
    [m_activeCommandBuffer presentDrawable: drawable];
    [m_activeCommandBuffer commit];
}

void MetalRenderingAPI::Draw(VertexArray* vertexArray)
{
    JKORN_ENGINE_ASSERT(false, "Not Implemented Yet.");
}

void MetalRenderingAPI::Draw(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
{
    JKORN_ENGINE_ASSERT(m_renderCommandEncoder != nil, "The render encoder doesn't exist.");
    
    MetalVertexBuffer* mtlVtxBuf = dynamic_cast<MetalVertexBuffer*>(vertexBuffer);
    MetalIndexBuffer* mtlIdxBuf = dynamic_cast<MetalIndexBuffer*>(indexBuffer);
    
    // Binds the vertex buffer.
    mtlVtxBuf->Bind();
    
    if (mtlIdxBuf != nullptr && mtlIdxBuf->IsValid())
    {
        MTLIndexType indexType = mtlIdxBuf->GetIndexType<MTLIndexType>();
        [m_renderCommandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:mtlIdxBuf->GetNumIndices() indexType:indexType indexBuffer:mtlIdxBuf->GetMTLBufferPtr() indexBufferOffset:0];
    }
    else
    {
        // Draws the primitives.
        [m_renderCommandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:mtlVtxBuf->GetNumVerts()];
    }
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
    JKORN_ENGINE_ASSERT(m_renderCommandEncoder, "The render pass descriptor is null.");
    // Sets the current fragment texture.
    [m_renderCommandEncoder setFragmentTexture:nullptr atIndex:(NSInteger)slot];
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
