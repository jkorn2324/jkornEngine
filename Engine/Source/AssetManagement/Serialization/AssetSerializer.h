#pragma once

#include <string>

// Here is messes it up, may need to add Rapidjson as a library to Editor.
#include "JsonFileParser.h"
#include "JsonUtils.h"
#include "Profiler.h"

#include <filesystem>

namespace Engine
{
	class GUID;

	struct AssetDeserializationMetaData
	{
		rapidjson::Value& value;
	};

	struct AssetDeserializationFileData
	{
		std::filesystem::path filePath;
	};

	struct AssetSerializationMetaData
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer>& prettyWriter;
		std::wstring filePath;
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
	public:
		AssetSerializer(T& asset)
			: m_asset(asset) { }

		// TODO: Need to deserialize from a GUID
		bool DeserializeFromGUID(const GUID& guid)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetFilePathFromGUID(guid, outputPath))
			{
				return false;
			}
			return DeserializeFromFile(outputPath);
		}

		template<typename...Args>
		bool DeserializeFromGUID(const GUID& guid, Args&&...args)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetFilePathFromGUID(guid, outputPath))
			{
				return false;
			}
			return DeserializeFromFile(outputPath, std::forward<Args>(args)...);
		}

		bool DeserializeFromFile(const std::filesystem::path& filePath)
		{
			return DeserializeFromFilePanel(filePath);
		}

		template<typename...Args>
		bool DeserializeFromFile(const std::wstring& filePath, Args&&...args)
		{
			return DeserializeFromFileFinal(filePath, std::forward<Args>(args)...);
		}

	private:
		template<typename...Args>
		bool DeserializeFromFileFinal(const std::wstring& filePath, Args&&...args)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData, std::forward<Args>(args)...);
		}

		bool DeserializeFromFileFinal(const std::wstring& filePath)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData);
		}

	private:
		T& m_asset;
	};
}