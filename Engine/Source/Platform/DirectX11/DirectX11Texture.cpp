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
	
	DirectX11Texture::~DirectX11Texture()
	{
		Free();
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

		GraphicsRenderer* graphicsRenderer = GraphicsRenderer::Get();
		DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
			graphicsRenderer->GetRenderingAPI());

		HRESULT result;
		std::wstring wstringPath(texturePath);
		std::wstring extension = wstringPath.substr(wstringPath.find_last_of('.'));
		if (extension == L".dds"
			|| extension == L".DDS")
		{
			result = DirectX::CreateDDSTextureFromFile(renderingAPI->m_device,
				texturePath, &m_texture, &m_shaderResourceView);
		}
		else
		{
			result = DirectX::CreateWICTextureFromFile(renderingAPI->m_device,
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
		GraphicsRenderer* graphics = GraphicsRenderer::Get();
		DirectX11RenderingAPI* renderingAPI = dynamic_cast<DirectX11RenderingAPI*>(
			graphics->GetRenderingAPI());
		renderingAPI->m_deviceContext->PSSetShaderResources(
			slot, 1, &m_shaderResourceView);
	}
}