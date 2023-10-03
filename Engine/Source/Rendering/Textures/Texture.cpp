#include "EnginePCH.h"
#include "Texture.h"

#include <filesystem>

#include "RenderingAPI.h"
#include "AssetSerializer.h"

#if defined(GRAPHICS_API_DIRECTX11)
#include "DirectX11Texture.h"
#endif

namespace Engine
{

	Texture::Texture()
		: m_width(0), m_height(0), m_serializedData() { }

	Texture::Texture(const TextureSpecifications& specifications)
		: m_width(specifications.width), m_height(specifications.height), m_serializedData(specifications.readWriteFlags, specifications.textureFormat)
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
		// TODO: Implementation
		return nullptr;
	}

	Texture* Texture::CreateTexture(const std::wstring& filePath)
	{
		// TODO: Implementation
		Texture* texture = nullptr;
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

	bool Texture::Load(const wchar_t* texturePath)
	{
		return Load(texturePath, m_serializedData);
	}

	bool Texture::DeserializeFromFile(Texture& texture, AssetDeserializationFileData& value)
	{
        std::wstring ws = value.filePath.wstring();
        return texture.Load(ws.data());
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
			
			fileWriter.BeginObject("ReadWriteFlags");
			fileWriter.Write<bool>("Readable_CPU", texture.m_serializedData.readWriteFlags & Flag_CPU_ReadTexture);
			fileWriter.Write<bool>("Writable_CPU", texture.m_serializedData.readWriteFlags & Flag_CPU_WriteTexture);
			fileWriter.Write<bool>("Readable_GPU", texture.m_serializedData.readWriteFlags & Flag_GPU_ReadTexture);
			fileWriter.Write<bool>("Writable_GPU", texture.m_serializedData.readWriteFlags & Flag_GPU_WriteTexture);
			fileWriter.EndObject();

			fileWriter.Write<uint32_t>("TextureFormat", texture.m_serializedData.textureFormat);
			
			fileWriter.Flush();
		}
		return true;
	}

	bool Texture::DeserializeMetaFile(Texture& texture, AssetDeserializationMetaFileData& metaData)
	{
		JsonFileParser fileReader(metaData.metaFilePath);
		if (!fileReader.IsValid()) return false;

		const auto& document = fileReader.GetDocument();
		if (document.HasMember("ReadWriteFlags"))
		{
			uint32_t deserializedRWFlags = 0;
			const auto& readWriteFlags = document["ReadWriteFlags"].GetObject();
			if (readWriteFlags.HasMember("Readable_CPU")
				&& readWriteFlags["Readable_CPU"].GetBool())
			{
				deserializedRWFlags |= Flag_CPU_ReadTexture;
			}
			if (readWriteFlags.HasMember("Readable_GPU")
				&& readWriteFlags["Readable_CPU"].GetBool())
			{
				deserializedRWFlags |= Flag_GPU_ReadTexture;
			}
			if (readWriteFlags.HasMember("Writable_CPU")
				&& readWriteFlags["Writable_CPU"].GetBool())
			{
				deserializedRWFlags |= Flag_CPU_WriteTexture;
			}
			if (readWriteFlags.HasMember("Writable_GPU")
				&& readWriteFlags["Writable_GPU"].GetBool())
			{
				deserializedRWFlags |= Flag_GPU_WriteTexture;
			}
			texture.m_serializedData.readWriteFlags 
				= (TextureReadWriteFlags)deserializedRWFlags;
		}

		if (document.HasMember("TextureFormat")
			&& document["TextureFormat"].IsUint())
		{
			texture.m_serializedData.textureFormat =
				(TextureFormat)document["TextureFormat"].GetUint();
		}
		return true;
	}


	bool Texture::Create(Texture** texture)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *texture = new DirectX11Texture();
        return true;
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::Create(Texture** texture, const TextureSpecifications& specifications)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        *texture = new DirectX11Texture(specifications);
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
#else
        JKORN_ENGINE_ASSERT(false, "Unsupported rendering API type.");
        return false;
#endif
	}

	bool Texture::Create(std::shared_ptr<Texture>& texture, const TextureSpecifications& specifications)
	{
#if defined(GRAPHICS_API_DIRECTX11)
        texture = std::make_shared<DirectX11Texture>(specifications);
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
}
