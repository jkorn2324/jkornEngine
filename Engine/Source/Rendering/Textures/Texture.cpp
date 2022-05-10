#include "EnginePCH.h"
#include "Texture.h"

#include <filesystem>

#include "RenderingAPI.h"
#include "DirectX11Texture.h"
#include "AssetSerializer.h"

namespace Engine
{

	static const int c_DefaultTextureRWFlags = Flag_GPU_ReadTexture | Flag_GPU_ReadTexture;

	Texture::Texture()
		: m_width(0), m_height(0), m_readWriteFlags((TextureReadWriteFlags)c_DefaultTextureRWFlags) { }

	Texture::Texture(const TextureSpecifications& specifications)
		: m_width(specifications.width), m_height(specifications.height), m_readWriteFlags(specifications.readWriteFlags)
	{
	}

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
		return texture.Load(value.filePath.c_str());
	}

	bool Texture::SerializeToFile(Texture& texture, AssetSerializationFileData& metaData)
	{
		return true;
	}

	bool Texture::SerializeToMetaFile(Texture& texture, AssetSerializationMetaFileData& metaData)
	{
		// Writes to a file path.
		{
			JsonFileWriter fileWriter(metaData.metaFilePath);
			fileWriter.Write<uint64_t>("GUID", (uint64_t)metaData.guid);
			fileWriter.Flush();
		}
		return true;
	}

	bool Texture::DeserializeMetaFile(Texture& texture, AssetDeserializationMetaFileData& metaData)
	{
		return true;
	}

	Texture* Texture::Create(const TextureSpecifications& specifications)
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11: return new DirectX11Texture(specifications);
		}
		DebugAssert(false, "Unsupported Texture type.");
		return nullptr;
	}

	Texture* Texture::Create()
	{
		switch (RenderingAPI::GetRenderingAPIType())
		{
		case RenderingAPIType::DIRECTX11:	return new DirectX11Texture();
		}
		DebugAssert(false, "Unsupported Texture type.");
		return nullptr;
	}
}