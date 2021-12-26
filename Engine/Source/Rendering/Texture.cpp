#include "EnginePCH.h"
#include "Texture.h"

#include "GraphicsRenderer.h"

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace Engine
{

	Texture::Texture()
		: m_texture(nullptr),
		m_shaderResourceView(nullptr),
		m_width(0),
		m_height(0)
	{
		// TODO:
	}

	Texture::~Texture()
	{
		Free();
	}
	
	void Texture::Free()
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

	Texture* Texture::StaticLoad(const std::wstring& texturePath)
	{
		Texture* texture = new Texture();
		const wchar_t* cstrPath = texturePath.c_str();
		if (!texture->Load(cstrPath))
		{
			delete texture;
			return nullptr;
		}
		return texture;
	}

	bool Texture::Load(const wchar_t* texturePath)
	{
		Free();

		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();

		HRESULT result;
		std::wstring wstringPath(texturePath);
		std::wstring extension = wstringPath.substr(wstringPath.find_last_of('.'));
		if (extension == L".dds"
			|| extension == L".DDS")
		{
			result = DirectX::CreateDDSTextureFromFile(graphicsRenderer->m_device,
				texturePath, &m_texture, &m_shaderResourceView);
		}
		else
		{
			result = DirectX::CreateWICTextureFromFile(graphicsRenderer->m_device,
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

	void Texture::Bind(std::uint32_t slot) const
	{
		if (m_texture != nullptr
			&& m_shaderResourceView != nullptr)
		{
			return;
		}
		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		graphics->m_deviceContext->PSSetShaderResources(
			slot, 1, &m_shaderResourceView);
	}
}