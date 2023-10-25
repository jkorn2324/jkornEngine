#include "EnginePCH.h"
#include "MetalIndexBuffer.h"

#include <memory>
#include <Metal/Metal.h>

#include "MetalRenderingAPI.h"
#include "GraphicsRenderer.h"

namespace Engine
{

MetalIndexBuffer::MetalIndexBuffer(const void* buffer, uint32_t numIndices, uint32_t stride)
    : IndexBuffer(buffer, numIndices, stride), m_bufferPtr(nullptr)
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    MTLResourceOptions metalResourceOptions = MTLResourceStorageModePrivate | MTLResourceCPUCacheModeWriteCombined;
    NSInteger len = (NSInteger)(numIndices * stride);
    m_bufferPtr = [renderingAPI.m_device newBufferWithBytes:buffer length:len options:metalResourceOptions];
    JKORN_ENGINE_ASSERT(m_bufferPtr, "Failed to create a metal buffer");
}

MetalIndexBuffer::~MetalIndexBuffer()
{
    if (m_bufferPtr)
    {
        [m_bufferPtr release];
    }
    m_bufferPtr = nullptr;
}

bool MetalIndexBuffer::IsValid() const
{
    return m_bufferPtr;
}

void MetalIndexBuffer::SetData(const void *buffer, uint32_t numIndices, uint32_t stride)
{
    if (!IsValid())
    {
        return;
    }
    void* bufPtr = [m_bufferPtr contents];
    std::memcpy(bufPtr, buffer, numIndices * stride);
}

void MetalIndexBuffer::Bind() const
{
    // TODO: Implementation, Set the Index Buffers
}

}
