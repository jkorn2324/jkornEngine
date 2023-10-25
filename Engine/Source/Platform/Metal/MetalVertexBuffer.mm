#include "EnginePCH.h"
#include "MetalVertexBuffer.h"

#include "MetalRenderingAPI.h"
#include "GraphicsRenderer.h"

#include <memory>
#include <Metal/Metal.h>

namespace Engine
{

MetalVertexBuffer::MetalVertexBuffer(const void* buffer, uint32_t numVertices, uint32_t stride)
    : VertexBuffer(buffer, numVertices, stride), m_bufferPtr(nullptr)
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    MTLResourceOptions metalResourceOptions = MTLResourceStorageModePrivate | MTLResourceCPUCacheModeWriteCombined;
    NSInteger len = (NSInteger)(numVertices * stride);
    m_bufferPtr = [renderingAPI.m_device newBufferWithBytes:buffer length:len options:metalResourceOptions];
    JKORN_ENGINE_ASSERT(m_bufferPtr, "Failed to create a metal buffer");
}

MetalVertexBuffer::~MetalVertexBuffer()
{
    // Releases the buffer pointer.
    if (m_bufferPtr)
    {
        [m_bufferPtr release];
    }
    m_bufferPtr = nil;
}

bool MetalVertexBuffer::IsValid() const
{
    return m_bufferPtr;
}

void MetalVertexBuffer::SetData(const void *buffer, uint32_t numVertices, uint32_t stride)
{
    if (!IsValid())
    {
        return;
    }
    // Sets the data for the vertex buffer.
    void* contentPtr = [m_bufferPtr contents];
    std::memcpy(contentPtr, buffer, numVertices * stride);
}
    
void MetalVertexBuffer::Bind() const
{
    if (!IsValid())
    {
        return;
    }
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    // TODO: Sets the active vertex buffer being drawn to the rendering api.
    // SetVertexBuffer (based on MTL Buffer)
}

}
