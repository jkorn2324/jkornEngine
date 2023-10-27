#include "EnginePCH.h"
#include "VertexBuffer.h"

#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11VertexBuffer.h"
#endif

#if defined(GRAPHICS_API_METAL)
#include "MetalVertexBuffer.h"
#endif

namespace Engine
{

	bool VertexBuffer::Create(VertexBuffer** ptr,
		const void* bufferData, uint32_t numVertices, uint32_t stride)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *ptr = new DirectX11VertexBuffer(bufferData, numVertices, stride);
        return true;
#elif defined(GRAPHICS_API_METAL)
        *ptr = new MetalVertexBuffer(bufferData, numVertices, stride);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Vertex Buffer type.");
        return false;
#endif
	}

	bool VertexBuffer::Create(std::shared_ptr<VertexBuffer>& ptr,
		const void* bufferData, uint32_t numVertices, uint32_t stride)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        ptr = std::make_shared<DirectX11VertexBuffer>(bufferData, numVertices, stride);
        return true;
#elif defined(GRAPHICS_API_METAL)
        ptr = std::make_shared<MetalVertexBuffer>(bufferData, numVertices, stride);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported Vertex Buffer type.");
        return false;
#endif
	}

	VertexBuffer::VertexBuffer(const void* buffer, uint32_t numVerts, uint32_t stride)
		: m_stride(stride), m_numVerts(numVerts), m_bufferLayoutParameters()
	{
	}

	uint32_t VertexBuffer::GetStride() const
	{
		return m_stride;
	}

	void VertexBuffer::SetBufferLayoutParameters(const BufferLayoutParameterSet& parameters)
	{
		m_bufferLayoutParameters = parameters;
	}

	uint32_t VertexBuffer::GetNumVerts() const
	{
		return m_numVerts;
	}
}
