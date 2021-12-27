#include "EnginePCH.h"
#include "ConstantBuffer.h"
#include "GraphicsRenderer.h"

#include <d3d11.h>

namespace Engine
{
	ConstantBuffer::ConstantBuffer(const void* buffer, std::size_t stride)
		: m_constantBuffer(nullptr)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = stride;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = buffer;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		GraphicsRenderer* renderer = GraphicsRenderer::Get();
		DebugAssert(renderer != nullptr, "Graphics renderer doesn't exist.");

		HRESULT result = renderer->m_device->CreateBuffer(&bufferDesc,
			&subresourceData, &m_constantBuffer);
		DebugAssert(result == S_OK, "Failed to create constant buffer.");

		Update(buffer, stride);
	}
	
	ConstantBuffer::~ConstantBuffer()
	{
		if (m_constantBuffer != nullptr)
		{
			m_constantBuffer->Release();
		}
	}

	void ConstantBuffer::Update(const void* data, std::size_t stride)
	{
		if (m_constantBuffer != nullptr)
		{
			GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();
			// Maps the subresource to the device context.
			D3D11_MAPPED_SUBRESOURCE mapResource;
			HRESULT result = graphicsRenderer->m_deviceContext->Map(
				m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
			DebugAssert(result == S_OK, "Failed to map the vertex buffer resource.");
			std::memcpy(mapResource.pData, data, stride);
			graphicsRenderer->m_deviceContext->Unmap(m_constantBuffer, 0);
		}
	}

	void ConstantBuffer::Bind(const std::uint32_t& slot, int flags) const
	{
		if (m_constantBuffer == nullptr)
		{
			return;
		}

		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		if (flags & ConstantBufferFlags::VERTEX_SHADER)
		{
			graphics->m_deviceContext->VSSetConstantBuffers(slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::PIXEL_SHADER)
		{
			graphics->m_deviceContext->PSSetConstantBuffers(slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::COMPUTE_SHADER)
		{
			graphics->m_deviceContext->CSSetConstantBuffers(slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::HULL_SHADER)
		{
			graphics->m_deviceContext->HSSetConstantBuffers(slot, 1, &m_constantBuffer);
		}
	}
}