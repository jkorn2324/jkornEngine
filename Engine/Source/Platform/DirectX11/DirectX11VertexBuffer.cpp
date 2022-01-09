#include "EnginePCH.h"
#include "DirectX11VertexBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

namespace Engine
{
	DirectX11VertexBuffer::DirectX11VertexBuffer(const void* buffer, 
		std::uint32_t numVertices, std::uint32_t stride)
		: VertexBuffer(buffer, numVertices, stride)
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = numVertices * stride;

		D3D11_SUBRESOURCE_DATA initializationData;
		initializationData.pSysMem = buffer;
		initializationData.SysMemPitch = 0;
		initializationData.SysMemSlicePitch = 0;

		HRESULT result = renderingAPI.m_device
			->CreateBuffer(&bufferDesc, &initializationData, &m_vertexBuffer);
		DebugAssert(result == S_OK, "Failed to create vertex buffer.");
		SetData(buffer, numVertices, stride);
	}

	DirectX11VertexBuffer::~DirectX11VertexBuffer()
	{
		if (m_vertexBuffer != nullptr)
		{
			m_vertexBuffer->Release();
		}
	}

	bool DirectX11VertexBuffer::IsValid() const
	{
		return m_vertexBuffer != nullptr;
	}

	void DirectX11VertexBuffer::SetData(const void* buffer, std::uint32_t numVertices, std::uint32_t stride)
	{
		m_numVerts = numVertices;
		m_stride = stride;

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();

		// Maps the subresource to the device context.
		D3D11_MAPPED_SUBRESOURCE mapResource;
		HRESULT result = renderingAPI.m_deviceContext->Map(
			m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
		DebugAssert(result == S_OK, "Failed to map the vertex buffer resource.");
		std::memcpy(mapResource.pData, buffer, numVertices * stride);
		renderingAPI.m_deviceContext->Unmap(m_vertexBuffer, 0);
	}

	void DirectX11VertexBuffer::Bind() const
	{
		if (!IsValid())
		{
			return;
		}
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();
		unsigned int offsets = 0;
		std::uint32_t stride = GetStride();
		renderingAPI.m_deviceContext->IASetVertexBuffers(0, 1,
			&m_vertexBuffer, &stride, &offsets);
	}
}