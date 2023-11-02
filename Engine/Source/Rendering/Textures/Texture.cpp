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

	bool Texture::Create(Texture** texture)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *texture = new DirectX11Texture();
        return true;
#elif defined(GRAPHICS_API_METAL)
        *texture = (Texture*)(new MetalTexture());
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::Create(Texture** texture, uint32_t width, uint32_t height, const TextureSpecifications& specifications)
	{
#if defined(GRAPHICS_API_DIRECTX11)
		*texture = new DirectX11Texture(width, height, specifications);
        return true;
#elif defined(GRAPHICS_API_METAL)
        *texture = new MetalTexture(width, height, specifications);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::Create(std::shared_ptr<Texture>& texture)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        texture = std::make_shared<DirectX11Texture>();
        return true;
#elif defined(GRAPHICS_API_METAL)
        texture = std::make_shared<MetalTexture>();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::Create(std::shared_ptr<Texture>& texture, uint32_t width, uint32_t height,
		const TextureSpecifications& specifications)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        texture = std::make_shared<DirectX11Texture>(width, height, specifications);
        return true;
#elif defined(GRAPHICS_API_METAL)
        texture = std::make_shared<MetalTexture>(width, height, specifications);
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::CopyTexture(Texture& a, Texture& b)
	{
		return a.CopyTo(b);
	}

	bool CopyTexture(Texture& a, Texture& b)
	{
		return a.CopyTo(b);
	}

	bool Texture::LoadFromFile(Texture** texture, const wchar_t* texturePath)
	{
		return LoadFromFile(texture, texturePath, c_defaultSpecification);
	}

	bool Texture::LoadFromFile(Texture** texture, const wchar_t* texturePath,
		const TextureSpecifications& specifications)
	{
		if (!Create(texture)) return false;
		Texture* textureReference = *texture;
		if (!textureReference->LoadFromFile_Internal(texturePath, specifications))
		{
			delete textureReference;
			*texture = nullptr;
			return false;
		}
		return true;
	}

	bool Texture::LoadFromFile(std::shared_ptr<Texture>& texture,
		const wchar_t* texturePath)
	{
		return LoadFromFile(texture, texturePath, c_defaultSpecification);
	}

	bool Texture::LoadFromFile(std::shared_ptr<Texture>& texture,
		const wchar_t* texturePath, const TextureSpecifications& specifications)
	{
		if (!Create(texture)) return false;
		if (!texture->LoadFromFile_Internal(texturePath, specifications))
		{
			texture.reset();
			return false;
		}
		return false;
	}
}
