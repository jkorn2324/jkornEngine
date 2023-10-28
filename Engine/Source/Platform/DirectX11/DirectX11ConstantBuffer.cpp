#include "EnginePCH.h"
#include "DirectX11ConstantBuffer.h"

#include "Memory.h"
#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Utils.h"

#include <d3d11.h>

namespace Engine
{
	DirectX11ConstantBuffer::DirectX11ConstantBuffer(const void* buffer, std::size_t stride)
		: ConstantBuffer(buffer, stride)
	{

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());
		m_constantBuffer = DirectX11Utils::CreateConstantBuffer(renderingAPI.m_device,
			buffer, stride);
		SetData((void*)buffer, stride);
	}

	DirectX11ConstantBuffer::~DirectX11ConstantBuffer()
	{
		if (m_constantBuffer != nullptr)
		{
			m_constantBuffer->Release();
		}
	}

	void DirectX11ConstantBuffer::SetData(void* buffer, std::size_t stride)
	{
		if (m_constantBuffer != nullptr
			&& buffer != nullptr)
		{
			DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
				GraphicsRenderer::GetRenderingAPI());

			// Maps the subresource to the device context.
			D3D11_MAPPED_SUBRESOURCE mapResource;
			HRESULT result = renderingAPI.m_deviceContext->Map(
				m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
            JKORN_ENGINE_ASSERT(result == S_OK, "Failed to map the vertex buffer resource.");
			Memory::Memcpy(mapResource.pData, buffer, stride);
			renderingAPI.m_deviceContext->Unmap(m_constantBuffer, 0);
		}
	}

	void DirectX11ConstantBuffer::Bind(const std::uint32_t& slot, int flags) const
	{
		if (m_constantBuffer == nullptr)
		{
			return;
		}

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)(
			GraphicsRenderer::GetRenderingAPI());

		if (flags & ConstantBufferFlags::VERTEX_SHADER)
		{
			renderingAPI.m_deviceContext->VSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::PIXEL_SHADER)
		{
			renderingAPI.m_deviceContext->PSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::COMPUTE_SHADER)
		{
			renderingAPI.m_deviceContext->CSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}

		if (flags & ConstantBufferFlags::HULL_SHADER)
		{
			renderingAPI.m_deviceContext->HSSetConstantBuffers(
				slot, 1, &m_constantBuffer);
		}
	}
}
