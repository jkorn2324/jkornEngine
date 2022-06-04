#include "EnginePCH.h"
#include "DirectX11VertexArray.h"
#include "DirectX11VertexBuffer.h"

#include "DirectX11RenderingAPI.h"
#include "GraphicsRenderer.h"
#include "IndexBuffer.h"

#include <d3d11.h>

namespace Engine
{
	DirectX11VertexArray::DirectX11VertexArray()
		: VertexArray::VertexArray(), 
		m_vertexBuffers()
	{
	}

	DirectX11VertexArray::~DirectX11VertexArray()
	{
	}

	void DirectX11VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& inVertexBuffer)
	{
		if (inVertexBuffer == nullptr) return;
		DirectX11VertexBuffer* vertexBuffer = reinterpret_cast<DirectX11VertexBuffer*>(inVertexBuffer.get());
		const auto& found = std::find(m_vertexBuffers.begin(), m_vertexBuffers.end(), inVertexBuffer);
		if (found == m_vertexBuffers.end())
		{
			m_vertexBuffers.push_back(inVertexBuffer);
		}
	}

	void DirectX11VertexArray::ClearVertexBuffers()
	{
		m_vertexBuffers.clear();
	}

	bool DirectX11VertexArray::GetVertexBuffer(std::shared_ptr<VertexBuffer>& buf, uint32_t index) const
	{
		if (index >= m_vertexBuffers.size())
		{
			buf = m_vertexBuffers[index];
			return true;
		}
		return false;
	}

	
	void DirectX11VertexArray::Bind() const
	{
		if (!IsValid()) return;
		
		static const uint32_t s_numVertexBuffers = 32;
		static UINT s_vertexBufferOffsets[s_numVertexBuffers] = { 0 };
		
		ID3D11Buffer* vertexBuffers[s_numVertexBuffers] = { };
		UINT vertexBufferStrides[s_numVertexBuffers] = { };

		uint8_t vertexBufferSize = (uint8_t)GetNumVertexBuffers();
		for (uint8_t i = 0; i < vertexBufferSize; ++i)
		{
			DirectX11VertexBuffer* vertexBuffer = reinterpret_cast<DirectX11VertexBuffer*>(
				m_vertexBuffers[i].get());
			vertexBuffers[i] = vertexBuffer->GetID3D11Buffer();
			vertexBufferStrides[i] = vertexBuffer->GetStride();
		}
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
		renderingAPI.m_deviceContext->IASetVertexBuffers(0, (uint16_t)vertexBufferSize,
			&vertexBuffers[0], &vertexBufferStrides[0], &s_vertexBufferOffsets[0]);
		m_indexBuffer->Bind();
	}
}