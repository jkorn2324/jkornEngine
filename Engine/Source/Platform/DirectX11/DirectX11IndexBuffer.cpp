#include "EnginePCH.h"
#include "DirectX11IndexBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

namespace Engine
{

	DirectX11IndexBuffer::DirectX11IndexBuffer(const void* buffer, 
		std::uint32_t numIndices, std::uint32_t stride)
		: IndexBuffer(buffer, numIndices, stride)
	{
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = numIndices * stride;

		D3D11_SUBRESOURCE_DATA initializationData;
		initializationData.pSysMem = buffer;
		initializationData.SysMemPitch = 0;
		initializationData.SysMemSlicePitch = 0;

		HRESULT result = renderingAPI.m_device
			->CreateBuffer(&bufferDesc, &initializationData, &m_indexBuffer);
		DebugAssert(result == S_OK, "Failed to create index buffer.");

		SetData(buffer, numIndices, stride);
	}

	DirectX11IndexBuffer::~DirectX11IndexBuffer()
	{
		if (m_indexBuffer != nullptr)
		{
			m_indexBuffer->Release();
		}
	}

	bool DirectX11IndexBuffer::IsValid() const
	{
		return m_indexBuffer != nullptr;
	}

	void DirectX11IndexBuffer::SetData(const void* buffer, uint32_t numIndices, uint32_t stride)
	{
		m_indexStride = stride;
		m_numIndices = numIndices;

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = renderingAPI.m_deviceContext->Map(
			m_indexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		DebugAssert(result == S_OK, "Failed to map the index buffer");
		std::memcpy(mappedResource.pData, buffer, numIndices * stride);
		renderingAPI.m_deviceContext->Unmap(m_indexBuffer, 0);

		m_format = stride == sizeof(uint16_t) ?
			DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	}

	void DirectX11IndexBuffer::Bind() const
	{
		if (!IsValid())
		{
			return;
		}
		DirectX11RenderingAPI& api = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();
		unsigned int offset = 0;
		api.m_deviceContext->IASetIndexBuffer(
			m_indexBuffer, m_format, offset);
	}
}