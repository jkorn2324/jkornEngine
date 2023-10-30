#include "EnginePCH.h"
#include "DirectX11Texture.h"

#include "GraphicsRenderer.h"
#include "DirectX11RenderingAPI.h"
#include "DirectX11Utils.h"
#include "Buffer.h"
#include "Memory.h"

namespace Engine
{
	// Pixel Masks.
	static const uint32_t s_d3d11PxAlphaMask = 0xf000;
	static const uint32_t s_d3d11PxRedMask = 0x0f00;
	static const uint32_t s_d3d11PxGreenMask = 0x00f0;
	static const uint32_t s_d3d11PxBlueMask = 0x000f;


	static TextureFormat FromD3D11Format(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return TextureFormat_RGBA32;
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return TextureFormat_RGBA8;
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_TYPELESS:
			return TextureFormat_Int32;
		case DXGI_FORMAT_R32_FLOAT:
			return TextureFormat_Float32;
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SINT:
			return TextureFormat_Int16;
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SINT:
			return TextureFormat_Int8;
		}
		return TextureFormat_Unknown;
	}

	static DXGI_FORMAT ToD3D11Format(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat_ARGB32:
		case TextureFormat_RGBA32:
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;
		case TextureFormat_Float32:
			return DXGI_FORMAT_R32_FLOAT;
		case TextureFormat_Int32:
			return DXGI_FORMAT_R32_TYPELESS;
		case TextureFormat_Int16:
			return DXGI_FORMAT_R16_TYPELESS;
		case TextureFormat_Int8:
			return DXGI_FORMAT_R8_TYPELESS;
		case TextureFormat_RGBA8:
			return DXGI_FORMAT_R8G8B8A8_TYPELESS;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	DirectX11Texture::DirectX11Texture() : Texture(),
		m_shaderResourceView(nullptr),
		m_texture(nullptr),
		m_pixels(nullptr)
	{

	}

	DirectX11Texture::DirectX11Texture(uint32_t width, uint32_t height,
		const TextureSpecifications& specifications)
		: Texture(width, height, specifications),
		m_shaderResourceView(nullptr),
		m_texture(nullptr),
		m_pixels(nullptr)
	{
		m_texture = DirectX11Utils::CreateTexture2D(
			GetRenderingAPI().m_device, m_width, m_height,
			D3D11_BIND_SHADER_RESOURCE,
			ToD3D11Format(specifications.textureFormat), 0, specifications.readWriteFlags);
		m_shaderResourceView = DirectX11Utils::CreateTextureShaderResourceView(
			GetRenderingAPI().m_device, (ID3D11Texture2D*)m_texture);
		m_pixels = new uint32_t[m_width * m_height];
	}

	DirectX11Texture::DirectX11Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView)
		: Texture(),
		m_shaderResourceView(shaderResourceView),
		m_texture(resource),
		m_pixels(nullptr)
	{
		if (m_texture != nullptr)
		{
			CD3D11_TEXTURE2D_DESC textureDesc;
			((ID3D11Texture2D*)m_texture)->GetDesc(&textureDesc);
			m_width = textureDesc.Width;
			m_height = textureDesc.Height;
			// TODO: Get Read Write Flags

			UINT readWriteFlags = 0;
			if (textureDesc.CPUAccessFlags & D3D11_CPU_ACCESS_READ)
			{
				readWriteFlags |= Flag_CPU_ReadTexture;
			}
			if (textureDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
			{
				readWriteFlags |= Flag_CPU_WriteTexture;
			}
			if (textureDesc.Usage == D3D11_USAGE_IMMUTABLE)
			{
				readWriteFlags |= Flag_GPU_ReadTexture;
			}
			else if(textureDesc.Usage == D3D11_USAGE_DEFAULT)
			{
				readWriteFlags |= Flag_GPU_ReadTexture | Flag_GPU_WriteTexture;
			}
			m_specifications.readWriteFlags = (TextureReadWriteFlags)readWriteFlags;
			m_specifications.textureFormat = FromD3D11Format(textureDesc.Format);

			m_pixels = new uint32_t[m_width * m_height];
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
		if (m_pixels != nullptr)
		{
			delete[] m_pixels;
			m_pixels = nullptr;
		}
		if (m_shaderResourceView != nullptr)
		{
			m_shaderResourceView->Release();
			m_shaderResourceView = nullptr;
		}
		if (m_texture != nullptr)
		{
			m_texture->Release();
			m_texture = nullptr;
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

	void DirectX11Texture::Bind(std::uint32_t slot) const
	{
		if (!IsValid())
		{
			return;
		}
		GetRenderingAPI().m_deviceContext->PSSetShaderResources(
			slot, 1, &m_shaderResourceView);
	}

	void DirectX11Texture::UpdatePixels() const
	{
		if (!IsReadable() || !IsWritable()) return;
		if(m_pixels == nullptr) return;

		uint32_t height = GetHeight(), width = GetWidth();
		// Gather the mapped subresource.
		D3D11_MAPPED_SUBRESOURCE resource;
		{
			D3D11_MAP map = IsWritable() ? D3D11_MAP_WRITE : D3D11_MAP_READ;
			HRESULT result = GetRenderingAPI().m_deviceContext->Map(m_texture, 0, map, 0, &resource);
			if (result != S_OK) return;
		}

		const uint8_t* resources = reinterpret_cast<uint8_t*>(resource.pData);
		for (uint32_t row = 0; row < height; row++)
		{
			uint32_t rowStart = row * resource.RowPitch;
			for (uint32_t col = 0; col < width; col++)
			{
				uint32_t colStart = col * resource.DepthPitch;
				uint32_t indexOffset = rowStart + colStart;

				uint8_t red = resources[indexOffset];
				uint8_t green = resources[indexOffset + 1];
				uint8_t blue = resources[indexOffset + 2];
				uint8_t alpha = resources[indexOffset + 3];

				m_pixels[row * width + col] = alpha << 24 | red << 16 | green << 8 | blue;
			}
		}
		GetRenderingAPI().m_deviceContext->Unmap(m_texture, 0);
	}

	bool DirectX11Texture::GetPixel(uint32_t x, uint32_t y, MathLib::Vector4& pixel) const
	{
		if (!IsReadable()) return false;

		uint32_t width = GetWidth(), height = GetHeight();
		uint32_t index = y * width + x;
		if (index > (width * height)) return false;

		uint32_t pixelData = m_pixels[index];
		pixel.x = ((float)((pixelData & s_d3d11PxRedMask) >> 16)) / 255.0f;
		pixel.y = ((float)((pixelData & s_d3d11PxGreenMask) >> 8)) / 255.0f;
		pixel.z = ((float)((pixelData & s_d3d11PxBlueMask)) / 255.0f);
		pixel.w = ((float)((pixelData & s_d3d11PxAlphaMask) >> 24) / 255.0f);
		return true;
	}
	
	void DirectX11Texture::CopyPixels(BufferModifier& view) const
	{
		if (m_texture == nullptr) return;

		D3D11_TEXTURE2D_DESC desc;
		((ID3D11Texture2D*)m_texture)->GetDesc(&desc);

		ID3D11Texture2D* texture = DirectX11Utils::CreateTexture2D(GetRenderingAPI().m_device,
			GetWidth(), GetHeight(), 0, desc.Format, 0, CPU_Access_Read);
		if (texture == nullptr) return;
		GetRenderingAPI().m_deviceContext->CopyResource(texture, m_texture);

		D3D11_MAPPED_SUBRESOURCE resourceDesc;
		HRESULT result = GetRenderingAPI().m_deviceContext->Map(
			texture, 0, D3D11_MAP_READ, 0, &resourceDesc);
		if (result != S_OK)
		{
			texture->Release();
			return;
		}

		TextureFormat textureFormat = GetTextureFormat();
		if (textureFormat == TextureFormat_Unknown)
		{
			GetRenderingAPI().m_deviceContext->Unmap(
				texture, 0);
			texture->Release();
			return;
		}

		size_t formatSize = SizeOfFormat(textureFormat);
		if (resourceDesc.pData)
		{
			size_t h = (size_t)m_height;
			for (size_t i = 0; i < h; i++)
			{
				size_t dstOffset = m_width * formatSize * i;
				size_t srcOffset = resourceDesc.RowPitch * i;
				size_t size = m_width * formatSize;
				view.Set((BufferView::Byte*)resourceDesc.pData + srcOffset, size, dstOffset);
			}
		}
		GetRenderingAPI().m_deviceContext->Unmap(
			texture, 0);
		texture->Release();
	}

	void DirectX11Texture::SetPixel(uint32_t x, uint32_t y, const MathLib::Vector4& inPixel)
	{
		if (!IsWritable()) return;

		D3D11_MAPPED_SUBRESOURCE resource;
		{
			HRESULT result = GetRenderingAPI().m_deviceContext->Map(
				m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			if (result != S_OK) return;
		}

		uint32_t width = GetWidth(), height = GetHeight();
		uint32_t index = y * width + x;
		if (index > (width * height)) return;

		// Converts from a vec4 to a packed color integer.
		uint32_t& pixel = m_pixels[index];
		uint8_t red = (uint8_t)(inPixel.x * 255.0f);
		uint8_t green = (uint8_t)(inPixel.y * 255.0f);
		uint8_t blue = (uint8_t)(inPixel.z * 255.0f);
		uint8_t alpha = (uint8_t)(inPixel.w * 255.0f);
		pixel = ((uint32_t)alpha << 24) | ((uint32_t)red << 16) |
			((uint32_t)green << 8) | ((uint32_t)blue);

		// Puts the pixels into the data.
		uint8_t* pixelBuffer = reinterpret_cast<uint8_t*>(resource.pData);
		pixelBuffer[index] = red;
		pixelBuffer[index + 1] = green;
		pixelBuffer[index + 2] = blue;
		pixelBuffer[index + 3] = alpha;
		GetRenderingAPI().m_deviceContext->Unmap(m_texture, 0);
	}

	bool DirectX11Texture::CopyTo(Texture& texture)
	{
		if (!IsValid()) return false;
		// TODO: Implementation
		return true;
	}

	bool DirectX11Texture::LoadFromFile_Internal(const wchar_t* texturePath)
	{
		Free();

		if (!DirectX11Utils::LoadTextureFromFile(GetRenderingAPI().m_device,
			texturePath, m_specifications.readWriteFlags, &m_texture, &m_shaderResourceView, D3D10_BIND_SHADER_RESOURCE))
		{
			return false;
		}
		CD3D11_TEXTURE2D_DESC textureDesc;
		((ID3D11Texture2D*)m_texture)->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;
		size_t wh = (size_t)m_width * (size_t)m_height;
		m_pixels = new uint32_t[wh];
		UpdatePixels();
		return true;
	}
}