#include "EnginePCH.h"
#include "DirectX11Texture.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Utils.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace Engine
{

	DirectX11Texture::DirectX11Texture() : Texture(),
		m_shaderResourceView(nullptr),
		m_texture(nullptr)
	{
	}

	DirectX11Texture::DirectX11Texture(const TextureSpecifications& specifications) : Texture(specifications),
		m_shaderResourceView(nullptr),
		m_texture(nullptr)
	{
		// TODO: Must Provide more specifications so that user doesn't have to set it to
		// just a render target view.

		// Creates a texture by default as a render target view.
		m_texture = DirectX11Utils::CreateTexture2D(
			GetRenderingAPI().m_device, m_width, m_height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 0, specifications.readWriteFlags);
		m_shaderResourceView = DirectX11Utils::CreateTextureShaderResourceView(
			GetRenderingAPI().m_device, (ID3D11Texture2D*)m_texture);
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

	DirectX11RenderingAPI& DirectX11Texture::GetRenderingAPI()
	{
		return (DirectX11RenderingAPI&)GraphicsRenderer::GetRenderingAPI();
	}

	bool DirectX11Texture::IsValid() const
	{
		return m_shaderResourceView != nullptr
			&& m_texture != nullptr;
	}

	bool DirectX11Texture::Load(const wchar_t* texturePath)
	{
		Free();

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
			result = DirectX::CreateDDSTextureFromFile(GetRenderingAPI().m_device,
				texturePath, &m_texture, &m_shaderResourceView);
		}
		else
		{
			result = DirectX::CreateWICTextureFromFile(GetRenderingAPI().m_device,
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
		GetRenderingAPI().m_deviceContext->PSSetShaderResources(
			slot, 1, &m_shaderResourceView);
	}
}