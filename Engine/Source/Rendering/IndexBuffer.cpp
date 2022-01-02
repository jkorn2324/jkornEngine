#include "EnginePCH.h"
#include "IndexBuffer.h"
#include "DirectX11IndexBuffer.h"

#include "RenderingAPI.h"

namespace Engine
{

	IndexBuffer* IndexBuffer::Create(const void* buffer, std::uint32_t numIndices, std::uint32_t stride)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11IndexBuffer(buffer, numIndices, stride);
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Unsupported Index buffer type.");
			return nullptr;
		}
		}
		return nullptr;
	}

	IndexBuffer::IndexBuffer(const void* indexBuffer, std::uint32_t numIndices, std::size_t stride)
		: m_numIndices(numIndices), m_indexStride(stride)
	{
		
	}

	std::uint32_t IndexBuffer::GetNumIndices() const
	{
		return m_numIndices;
	}

	std::uint32_t IndexBuffer::GetStride() const
	{
		return m_indexStride;
	}

}