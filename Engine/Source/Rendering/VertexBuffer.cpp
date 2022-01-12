#include "EnginePCH.h"
#include "VertexBuffer.h"

#include "RenderingAPI.h"
#include "DirectX11VertexBuffer.h"

namespace Engine
{

	VertexBuffer* VertexBuffer::Create(const void* bufferData, uint32_t numVertices, uint32_t stride)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11VertexBuffer(bufferData, numVertices, stride);
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Unsupported Vertex Buffer type.");
			return nullptr;
		}
		}
		return nullptr;
	}

	VertexBuffer::VertexBuffer(const void* buffer, uint32_t numVerts, uint32_t stride)
		: m_stride(stride), m_numVerts(numVerts)
	{
	}

	uint32_t VertexBuffer::GetStride() const
	{
		return m_stride;
	}

	uint32_t VertexBuffer::GetNumVerts() const
	{
		return m_numVerts;
	}
}