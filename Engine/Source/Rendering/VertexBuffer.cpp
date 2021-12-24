#include "EnginePCH.h"
#include "VertexBuffer.h"
#include "Rendering\GraphicsRenderer.h"

namespace Engine
{

	VertexBuffer::VertexBuffer(const void* buffer, std::uint32_t numVerts, std::size_t stride)
		: m_vertexBuffer(nullptr)
	{
		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = numVerts * stride;

		D3D11_SUBRESOURCE_DATA initializationData;
		initializationData.pSysMem = buffer;
		initializationData.SysMemPitch = 0;
		initializationData.SysMemSlicePitch = 0;

		HRESULT result = graphicsRenderer->m_device
			->CreateBuffer(&bufferDesc, &initializationData, &m_vertexBuffer);
		DebugAssert(result == S_OK, "Failed to create vertex buffer.");

		if (buffer != nullptr)
		{
			// Maps the subresource to the device context.
			D3D11_MAPPED_SUBRESOURCE mapResource;
			result = graphicsRenderer->m_deviceContext->Map(
				m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
			DebugAssert(result == S_OK, "Failed to map the vertex buffer resource.");
			std::memcpy(mapResource.pData, buffer, numVerts * stride);
			graphicsRenderer->m_deviceContext->Unmap(m_vertexBuffer, 0);
		}

		m_stride = stride;
		m_numVerts = numVerts;
	}

	VertexBuffer::~VertexBuffer()
	{
		m_vertexBuffer->Release();
	}

	void VertexBuffer::Bind() const
	{
		if (!IsValid())
		{
			return;
		}

		unsigned int offsets = 0;
		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		graphics->m_deviceContext->IASetVertexBuffers(0, 1, 
			&m_vertexBuffer, &m_stride, &offsets);
	}

	bool VertexBuffer::IsValid() const
	{
		return m_vertexBuffer != nullptr;
	}

	std::uint32_t VertexBuffer::GetNumVerts() const
	{
		return m_numVerts;
	}
}