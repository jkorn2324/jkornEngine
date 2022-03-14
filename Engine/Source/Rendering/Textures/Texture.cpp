#include "EnginePCH.h"
#include "Texture.h"

#include "RenderingAPI.h"
#include "DirectX11Texture.h"
#include "AssetSerializer.h"

namespace Engine
{

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

	Texture* Texture::CreateTexture()
	{
		return Create();
	}

	Texture* Texture::CreateTexture(const std::wstring& filePath)
	{
		Texture* texture = Create();
		if(texture != nullptr)
		{
			Engine::AssetSerializer<Texture> serializer(*texture);
			if (serializer.DeserializeFromFile(filePath))
			{
				return texture;
			}
		}
		delete texture;
		return nullptr;
	}


	bool Texture::DeserializeFromFile(Texture& texture, AssetDeserializationFileData& value)
	{
		// TODO: Read from a normal file.
		return texture.Load(value.filePath.c_str());
	}

	bool Texture::SerializeToFile(Texture& texture, AssetSerializationMetaData& metaData)
	{
		// TODO: 
		return true;
	}

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
}