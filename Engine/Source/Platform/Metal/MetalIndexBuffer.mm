#include "EnginePCH.h"
#include "MetalIndexBuffer.h"

#include "Memory.h"
#include <Metal/Metal.h>

#include "MetalRenderingAPI.h"
#include "GraphicsRenderer.h"

namespace Engine
{

namespace
{

int8_t GetMetalIndexType(uint32_t stride)
{
    switch (stride)
    {
        case sizeof(uint32_t): return (uint8_t)MTLIndexTypeUInt32;
        case sizeof(uint16_t): return (uint8_t)MTLIndexTypeUInt16;
    }
    return -1;
}

bool IsValidIndexType(uint32_t stride)
{
    int8_t indexType = GetMetalIndexType(stride);
    return indexType != -1;
}

}

MetalIndexBuffer::MetalIndexBuffer(const void* buffer, uint32_t numIndices, uint32_t stride)
    : IndexBuffer(buffer, numIndices, stride), m_bufferPtr(nullptr)
{
    MetalRenderingAPI& renderingAPI = (MetalRenderingAPI&)GraphicsRenderer::GetRenderingAPI();
    MTLResourceOptions metalResourceOptions =  MTLResourceCPUCacheModeWriteCombined;
    NSInteger len = (NSInteger)(numIndices * stride);
    m_bufferPtr = [renderingAPI.GetDevice() newBufferWithBytes:buffer length:len options:metalResourceOptions];
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
    return m_bufferPtr && IsValidIndexType(m_indexStride);
}

void MetalIndexBuffer::SetData(const void *buffer, uint32_t numIndices, uint32_t stride)
{
    JKORN_ENGINE_ASSERT(IsValidIndexType(stride), "Stride must be a valid 16 bit or 32 bit integer.");
    
    if (!IsValid())
    {
        return;
    }
    void* bufPtr = [m_bufferPtr contents];
    Memory::Memcpy(bufPtr, buffer, numIndices * stride);
    m_indexStride = stride;
    m_numIndices = numIndices;
}

uint8_t MetalIndexBuffer::GetIndexTypeInternal() const { return GetMetalIndexType(m_indexStride); }



}
