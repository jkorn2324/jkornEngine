#include "EnginePCH.h"
#include "DirectX11ConstantBuffer.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

#include <d3d11.h>

namespace Engine
{
	DirectX11ConstantBuffer::DirectX11ConstantBuffer(const void* buffer, std::size_t stride)
		: ConstantBuffer(buffer, stride)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = (UINT)stride;
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
		DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
			renderer->GetRenderingAPI());

		HRESULT result = renderingAPI->m_device->CreateBuffer(&bufferDesc,
			&subresourceData, &m_constantBuffer);
		DebugAssert(result == S_OK, "Failed to create constant buffer.");

		SetData(buffer, stride);
	}

	DirectX11ConstantBuffer::~DirectX11ConstantBuffer()
	{
		if (m_constantBuffer != nullptr)
		{
			m_constantBuffer->Release();
		}
	}

	void DirectX11ConstantBuffer::SetData(const void* buffer, std::size_t stride)
	{
		if (m_constantBuffer != nullptr)
		{
			GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();
			DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
				graphicsRenderer->GetRenderingAPI());

			// Maps the subresource to the device context.
			D3D11_MAPPED_SUBRESOURCE mapResource;
			HRESULT result = renderingAPI->m_deviceContext->Map(
				m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
			DebugAssert(result == S_OK, "Failed to map the vertex buffer resource.");
			std::memcpy(mapResource.pData, buffer, stride);
			renderingAPI->m_deviceContext->Unmap(m_constantBuffer, 0);
		}
	}

	void DirectX11ConstantBuffer::Bind(const std::uint32_t& slot, int flags) const
	{
		if (m_constantBuffer == nullptr)
		{
			return;
		}

		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
			graphics->GetRenderingAPI());

		if (flags & ConstantBufferFlags::VERTEX_SHADER)
		{
			renderingAPI->m_deviceContext->VSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::PIXEL_SHADER)
		{
			renderingAPI->m_deviceContext->PSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::COMPUTE_SHADER)
		{
			renderingAPI->m_deviceContext->CSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::HULL_SHADER)
		{
			renderingAPI->m_deviceContext->HSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}
	}
}