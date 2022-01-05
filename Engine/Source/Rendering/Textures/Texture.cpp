#include "EnginePCH.h"
#include "Texture.h"

#include "RenderingAPI.h"
#include "DirectX11Texture.h"

namespace Engine
{

	Texture* Texture::Create()
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11Texture();
		case RenderingAPIType::NONE:
		{
			DebugAssert(false, "Unsupported Texture type.");
			return nullptr;
		}
		}
		return nullptr;
	}

	Texture* Texture::StaticLoad(const std::wstring& texturePath)
	{
		Texture* texture = Create();
		if (texture == nullptr)
		{
			return nullptr;
		}
		const wchar_t* cstrPath = texturePath.c_str();
		if (!texture->Load(cstrPath))
		{
			delete texture;
			return nullptr;
		}
		return texture;
	}

	Texture::Texture()
		: m_width(0), m_height(0) { }

	std::uint32_t Texture::GetWidth() const
	{
		return m_width;
	}

	std::uint32_t Texture::GetHeight() const
	{
		return m_height;
	}
}