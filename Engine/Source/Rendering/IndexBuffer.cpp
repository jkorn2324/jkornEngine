#include "EnginePCH.h"
#include "IndexBuffer.h"

#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11IndexBuffer.h"
#endif

#if defined(GRAPHICS_API_METAL)
#include "MetalIndexBuffer.h"
#endif

namespace Engine
{

	IndexBuffer::IndexBuffer(const void* indexBuffer, uint32_t numIndices, uint32_t stride)
		: m_numIndices(numIndices), m_indexStride(stride)
	{
		
	}

	uint32_t IndexBuffer::GetNumIndices() const
	{
		return m_numIndices;
	}

    uint32_t IndexBuffer::GetStride() const
	{
		return m_indexStride;
	}

	bool IndexBuffer::Create(std::shared_ptr<IndexBuffer>& buf, const void* buffer, uint32_t indices, uint32_t stride)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        buf = std::make_shared<DirectX11IndexBuffer>(buffer, indices, stride);
        return true;
#elif defined(GRAPHICS_API_METAL)
        buf = std::make_shared<MetalIndexBuffer>(buffer, indices, stride);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Index buffer type.");
        return false;
#endif
	}

	bool IndexBuffer::Create(std::unique_ptr<IndexBuffer>& buf, const void* buffer, uint32_t indices, uint32_t stride)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        buf = std::make_unique<DirectX11IndexBuffer>(buffer, indices, stride);
        return true;
#elif defined(GRAPHICS_API_METAL)
        buf = std::make_unique<MetalIndexBuffer>(buffer, indices, stride);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Index buffer type.");
        return false;
#endif
	}

	bool IndexBuffer::Create(IndexBuffer** buf, const void* buffer, uint32_t indices, uint32_t stride)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *buf = new DirectX11IndexBuffer(buffer, indices, stride);
        return true;
#elif defined(GRAPHICS_API_METAL)
        *buf = new MetalIndexBuffer(buffer, indices, stride);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Index buffer type.");
        return false;
#endif
	}

}
