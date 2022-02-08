#include "EnginePCH.h"
#include "DirectX11Texture.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace Engine
{
	DirectX11Texture::DirectX11Texture() : Texture(),
		m_shaderResourceView(nullptr),
		m_texture(nullptr)
	{
	}

	DirectX11Texture::DirectX11Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView)
		: Texture(),
		m_shaderResourceView(shaderResourceView),
		m_texture(resource)
	{
		if (m_texture != nullptr)
		{
			CD3D11_TEXTURE2D_DESC textureDesc;
			((ID3D11Texture2D*)m_texture)->GetDesc(&textureDesc);
			m_width = textureDesc.Width;
			m_height = textureDesc.Height;
		}
	}
	
	DirectX11Texture::~DirectX11Texture()
	{
		Free();
	}

	const void* DirectX11Texture::GetTextureID() const
	{
		if (m_shaderResourceView != nullptr)
		{
			return m_shaderResourceView;
		}
		return nullptr;
	}

	void DirectX11Texture::Free()
	{
		if (m_shaderResourceView != nullptr)
		{
			m_shaderResourceView->Release();
		}
		if (m_texture != nullptr)
		{
			m_texture->Release();
		}
		m_width = 0;
		m_height = 0;
	}

	bool DirectX11Texture::IsValid() const
	{
		return m_shaderResourceView != nullptr
			&& m_texture != nullptr;
	}

	bool DirectX11Texture::Load(const wchar_t* texturePath)
	{
		Free();

		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();

		HRESULT result;
		std::wstring wstringPath(texturePath);
		if (wstringPath.size() <= 0)
		{
			return false;
		}

		std::wstring extension = wstringPath.substr(wstringPath.find_last_of('.'));
		if (extension == L".dds"
			|| extension == L".DDS")
		{
			result = DirectX::CreateDDSTextureFromFile(renderingAPI.m_device,
				texturePath, &m_texture, &m_shaderResourceView);
		}
		else
		{
			result = DirectX::CreateWICTextureFromFile(renderingAPI.m_device,
				texturePath, &m_texture, &m_shaderResourceView);
		}

		DebugAssert(result == S_OK, "Failed to create texture appropriately.");
		if (result != S_OK)
		{
			return false;
		}
		CD3D11_TEXTURE2D_DESC textureDesc;
		((ID3D11Texture2D*)m_texture)->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;
		return true;
	}

	void DirectX11Texture::Bind(std::uint32_t slot) const
	{
		if (!IsValid())
		{
			return;
		}
		DirectX11RenderingAPI& renderingAPI = (DirectX11RenderingAPI&)
			GraphicsRenderer::GetRenderingAPI();
		renderingAPI.m_deviceContext->PSSetShaderResources(
			slot, 1, &m_shaderResourceView);
	}
}