#include "EnginePCH.h"
#include "VertexBuffer.h"

#include "RenderingAPI.h"
#include "DirectX11VertexBuffer.h"

namespace Engine
{

	bool VertexBuffer::Create(VertexBuffer** ptr,
		const void* bufferData, uint32_t numVertices, uint32_t stride)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				*ptr = new DirectX11VertexBuffer(bufferData, numVertices, stride);
				return true;
			}
		}
		DebugAssert(false, "Unsupported Vertex Buffer type.");
		return false;
	}

	bool VertexBuffer::Create(std::shared_ptr<VertexBuffer>& ptr,
		const void* bufferData, uint32_t numVertices, uint32_t stride)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
			case RenderingAPIType::DIRECTX11:
			{
				ptr = std::make_shared<DirectX11VertexBuffer>(bufferData, numVertices, stride);
				return true;
			}
		}
		DebugAssert(false, "Unsupported Vertex Buffer type.");
		return false;
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