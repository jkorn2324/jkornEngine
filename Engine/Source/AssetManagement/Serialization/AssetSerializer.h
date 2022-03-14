#pragma once

#include <string>

#include "JsonFileParser.h"
#include "JsonFileWriter.h"
#include "JsonUtils.h"
#include "Profiler.h"

#include <filesystem>

namespace Engine
{
	class GUID;

	struct AssetDeserializationFileData
	{
		std::filesystem::path filePath;
	};

	struct AssetSerializationMetaData
	{
		std::filesystem::path filePath;
	};

	// Used so that the implementations can be defined in a cpp file.
	struct AssetSerializerFuncs
	{
	private:
		static bool GetAssetPathFromGUID(const GUID& guid, std::filesystem::path& path);
		
		template<typename T>
		friend class AssetSerializer;
	};

	template<typename T>
	class AssetSerializer
	{
		// TODO: Separate In Editor stuff from outside of editor (possible in-editor macro?)

	public:
		AssetSerializer(T& asset)
			: m_asset(asset) { }

		bool SerializeToFile(const std::filesystem::path& path)
		{
			return SerializeToFileInternal(path);
		}

		bool SerializeFromGUID(const GUID& guid)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetAssetPathFromGUID(guid, outputPath))
			{
				return false;
			}
			return SerializeToFile(outputPath);
		}

		bool DeserializeFromGUID(const GUID& guid)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetAssetPathFromGUID(guid, outputPath))
			{
				return false;
			}
			return DeserializeFromFile(outputPath);
		}

		template<typename...Args>
		bool DeserializeFromGUID(const GUID& guid, Args&&...args)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetAssetPathFromGUID(guid, outputPath))
			{
				return false;
			}
			return DeserializeFromFile(outputPath, std::forward<Args>(args)...);
		}

		bool DeserializeFromFile(const std::filesystem::path& filePath)
		{
			return DeserializeFromFileInternal(filePath);
		}

		template<typename...Args>
		bool DeserializeFromFile(const std::filesystem::path& filePath, Args&&...args)
		{
			return DeserializeFromFileInternal(filePath, std::forward<Args>(args)...);
		}

	private:
		template<typename...Args>
		bool DeserializeFromFileInternal(const std::filesystem::path& filePath, Args&&...args)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData, std::forward<Args>(args)...);
		}

		bool DeserializeFromFileInternal(const std::filesystem::path& filePath)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData);
		}

		bool SerializeToFileInternal(const std::filesystem::path& filePath)
		{
			PROFILE_SCOPE_FUNC(T, SerializeToFile, Serialization);
			AssetSerializationMetaData serializerData = { filePath };
			return T::SerializeToFile(m_asset, serializerData);
		}

	private:
		T& m_asset;
	};
}