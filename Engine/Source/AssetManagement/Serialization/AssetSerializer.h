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

	struct AssetSerializerFuncs
	{
	private:
		static GUID GetGUID(const std::wstring& filePath);
		
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
			// TODO: Convert the GUID to an asset path given by the asset manager and load it from there.
			return true;
		}

		template<typename...Args>
		bool DeserializeFromGUID(const GUID& guid, Args&&...args)
		{
			// TODO: Convert the GUID to an asset path given by the asset manager and then call deserializeFromFileFinal
			return true;
		}

		bool DeserializeFromFile(const std::filesystem::path& filePath)
		{
			// TODO: Remove macro, only here because we want to ignore this code.
#ifdef LOAD_FROM_GUID
			const GUID guid = AssetSerializerFuncs::GetGUID(filePath);
			return DeserializeFromGUID(guid, std::forward<Args>(args)...);
#endif
			return DeserializeFromFilePanel(filePath);
		}

		template<typename...Args>
		bool DeserializeFromFile(const std::wstring& filePath, Args&&...args)
		{
			// TODO: Remove macro, only here because we want to ignore this code.
#ifdef LOAD_FROM_GUID
			const GUID guid = AssetSerializerFuncs::GetGUID(filePath);
			return DeserializeFromGUID(guid, std::forward<Args>(args)...);
#endif
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