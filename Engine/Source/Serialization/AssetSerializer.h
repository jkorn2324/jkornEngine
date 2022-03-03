#pragma once

#include <string>

// Here is messes it up, may need to add Rapidjson as a library to Editor.
#include "JsonFileParser.h"
#include "JsonUtils.h"
#include "Profiler.h"

namespace Engine
{

	struct AssetDeserializationMetaData
	{
		rapidjson::Value& value;
	};

	struct AssetDeserializationFileData
	{
		std::wstring filePath;
	};

	struct AssetSerializationMetaData
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer>& prettyWriter;
		std::wstring filePath;
	};

	template<typename T>
	class AssetSerializer
	{
	public:
		AssetSerializer(T& asset)
			: m_asset(asset) { }

		void SerializeToMetaFile(const std::wstring& filePath)
		{
			PROFILE_SCOPE(T, Serialization);

			rapidjson::StringBuffer assetBuffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(assetBuffer);

			prettyWriter.StartObject();

			AssetSerializationMetaData metaData = { prettyWriter, filePath };
			T::SerializeToMetaFile(m_asset, { prettyWriter, metaData });

			prettyWriter.EndObject();

			FILE* file;
			fopen_s(&file, filePath.c_str(), "w");
			DebugAssert(file != nullptr, "Something went wrong with serializing to a meta file.");
			fprintf_s(file, "%s", sceneBuffer.GetString());
			fclose(file);
		}

		bool DeserializeFromMetaFile(const rapidjson::Value& value)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromMeta, Serialization);
			AssetDeserializationMetaData metaData = { value };
			return T::DeserializeFromMetaFile(m_asset, metaData);
		}

		bool DeserializeFromFile(const std::wstring& filePath)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData);
		}

		template<typename...Args>
		bool DeserializeFromFile(const std::wstring& filePath, Args&&...args)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData, std::forward<Args>(args)...);
		}

	private:
		T& m_asset;
	};
}