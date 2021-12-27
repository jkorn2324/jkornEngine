#include "EnginePCH.h"
#include "IndexBuffer.h"
#include "Rendering\GraphicsRenderer.h"

namespace Engine
{

	IndexBuffer::IndexBuffer(const void* indexBuffer, std::uint32_t numIndices, std::size_t stride)
	{
		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = numIndices * stride;

		D3D11_SUBRESOURCE_DATA initializationData;
		initializationData.pSysMem = indexBuffer;
		initializationData.SysMemPitch = 0;
		initializationData.SysMemSlicePitch = 0;

		DebugAssert(graphicsRenderer != nullptr, "Graphics Renderer doesn't exist.");
		HRESULT result = graphicsRenderer->m_device
			->CreateBuffer(&bufferDesc, &initializationData, &m_indexBuffer);
		DebugAssert(result == S_OK, "Failed to create index buffer.");

		if (indexBuffer != nullptr)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			result = graphicsRenderer->m_deviceContext->Map(
				m_indexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			DebugAssert(result == S_OK, "Failed to map the index buffer");
			std::memcpy(mappedResource.pData, indexBuffer, stride * numIndices);
			graphicsRenderer->m_deviceContext->Unmap(m_indexBuffer, 0);
		}

		m_indexStride = stride;
		m_numIndices = numIndices;
		m_format = stride == sizeof(uint16_t) ? 
			DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	}

	IndexBuffer::~IndexBuffer()
	{
		m_indexBuffer->Release();
	}

	bool IndexBuffer::IsValid() const
	{
		return m_indexBuffer != nullptr;
	}

	std::uint32_t IndexBuffer::GetNumIndices() const
	{
		return m_numIndices;
	}

	void IndexBuffer::Bind() const
	{
		if (!IsValid())
		{
			return;
		}
		unsigned int offset = 0;
		GraphicsRenderer* renderer = GraphicsRenderer::Get();
		renderer->m_deviceContext->IASetIndexBuffer(
			m_indexBuffer, m_format, offset);
	}
}