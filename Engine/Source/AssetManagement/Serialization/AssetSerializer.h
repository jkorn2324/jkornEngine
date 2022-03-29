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

	struct AssetSerializationFileData
	{
		std::filesystem::path filePath;
		uint64_t guid;
	};

	struct AssetSerializationMetaFileData
	{
		std::filesystem::path metaFilePath;
		uint64_t guid;
	};

	struct AssetDeserializationMetaFileData
	{
		std::filesystem::path metaFilePath;
	};

	// Used so that the implementations can be defined in a cpp file.
	struct AssetSerializerFuncs
	{
	private:
		static bool GetAssetPathFromGUID(const GUID& guid, std::filesystem::path& path);
		static void SetAssetPath(const GUID& guid, const std::filesystem::path& path);
		
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

		AssetSerializer(T& asset, bool useMetaFile)
			: m_asset(asset), m_useMetaFile(useMetaFile) { }

		void SetUseMetaFile(bool useMetaFile)
		{
			m_useMetaFile = useMetaFile;
		}

		bool UseMetaFile() const
		{
			return m_useMetaFile;
		}

		bool SerializeToFile(GUID& guid, const std::filesystem::path& path)
		{
			return SerializeToFileInternal(guid, path);
		}

		bool SerializeToFile(const std::filesystem::path& path)
		{
			GUID guid;
			return SerializeToFile(guid, path);
		}

		bool SerializeFromGUID(const GUID& guid)
		{
			std::filesystem::path outputPath;
			if (!AssetSerializerFuncs::GetAssetPathFromGUID(guid, outputPath))
			{
				return false;
			}
			return SerializeToFileInternal(guid, outputPath);
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

			if (m_useMetaFile)
			{
				std::filesystem::path metaFilePath = filePath;
				metaFilePath += L".meta";
				AssetDeserializationMetaFileData metaDeserializerData = { metaFilePath };
				T::DeserializeMetaFile(m_asset, metaDeserializerData);
			}
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData, std::forward<Args>(args)...);
		}

		bool DeserializeFromFileInternal(const std::filesystem::path& filePath)
		{
			PROFILE_SCOPE_FUNC(T, DeserializeFromFile, Serialization);
			if (m_useMetaFile)
			{
				std::filesystem::path metaFilePath = filePath;
				metaFilePath += L".meta";
				AssetDeserializationMetaFileData deserializerData = { metaFilePath };
				T::DeserializeMetaFile(m_asset, deserializerData);
			}
			AssetDeserializationFileData deserializerData = { filePath };
			return T::DeserializeFromFile(m_asset, deserializerData);
		}

		bool SerializeToFileInternal(const GUID& guid, const std::filesystem::path& filePath)
		{
			PROFILE_SCOPE_FUNC(T, SerializeToFile, Serialization);
			AssetSerializationFileData serializerData = { filePath };
			bool serializeLiteral = T::SerializeToFile(m_asset, serializerData);
			if (!m_useMetaFile)
			{
				AssetSerializerFuncs::SetAssetPath(guid, filePath);
				return serializeLiteral;
			}
			if (serializeLiteral)
			{
				std::filesystem::path metaFilePath = filePath;
				metaFilePath += L".meta";
				AssetSerializationMetaFileData metaData = { metaFilePath, (uint32_t)guid };
				T::SerializeToMetaFile(m_asset, metaData);
				AssetSerializerFuncs::SetAssetPath(guid, filePath);
			}
			return serializeLiteral;
		}

	private:
		T& m_asset;
		bool m_useMetaFile;
	};
}