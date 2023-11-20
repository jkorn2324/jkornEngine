#include "EnginePCH.h"
#include "Texture.h"

#include <filesystem>

#include "RenderingAPI.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11Texture.h"
#endif

#if defined(GRAPHICS_API_METAL)
#include "MetalTexture.h"
#endif

namespace Engine
{

	static const TextureSpecifications c_defaultSpecification;

	Texture::Texture()
		: m_width(0), m_height(0), m_specifications() { }

	Texture::Texture(uint32_t width, uint32_t height, const TextureSpecifications& specifications)
		: m_width(width), m_height(height), m_specifications(specifications.readWriteFlags, specifications.textureFormat)
	{
	}

	uint32_t Texture::GetWidth() const
	{
		return m_width;
	}

	uint32_t Texture::GetHeight() const
	{
		return m_height;
	}

	bool Texture::LoadFromFile_Internal(const wchar_t* texturePath, const TextureSpecifications& specifications)
	{
		m_specifications = specifications;
		return LoadFromFile_Internal(texturePath);
	}

	bool Texture::CopyTexture(Texture& a, Texture& b)
	{
		return a.CopyTo(b);
	}

	bool CopyTexture(Texture& a, Texture& b)
	{
		return a.CopyTo(b);
	}

	Texture* Texture::Create(uint32_t width, uint32_t height, const TextureSpecifications& specifications, bool empty)
	{
#if defined(GRAPHICS_API_DIRECTX11)
		return empty ? new DirectX11Texture()
				: new DirectX11Texture(width, height, specifications);
#elif defined(GRAPHICS_API_METAL)
		return empty ? new MetalTexture()
				: new MetalTexture(width, height, specifications);
#else
		JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
		return nullptr;
#endif
	}

	Texture* Texture::LoadFromFile(const wchar_t* texturePath, const TextureSpecifications& specifications)
	{
		Texture* texturePtr = Create();
		if (!texturePtr)
		{
			return nullptr;
		}
		if (!texturePtr->LoadFromFile_Internal(texturePath))
		{
			delete texturePtr;
			return nullptr;
		}
		return texturePtr;
	}
}
