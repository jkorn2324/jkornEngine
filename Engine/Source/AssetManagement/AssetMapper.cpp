#include "EnginePCH.h"
#include "AssetMapper.h"

#include "JobManager.h"
#include "JsonFileParser.h"
#include "JsonUtils.h"
#include "JsonFileWriter.h"
#include "Profiler.h"

namespace Engine
{
	std::filesystem::path AssetMapper::GetPath(const GUID& guid)
	{
		const auto& outputPath = m_assetGUIDs.find(guid);
		if(outputPath != m_assetGUIDs.end())
		{
			return outputPath->second;
		}
		return std::filesystem::path();
	}

	void AssetMapper::SetPath(const GUID& guid, const std::filesystem::path& path)
	{
		const auto& outputPath = m_assetGUIDs.find(guid);
		if (outputPath != m_assetGUIDs.end())
		{
			m_assetGUIDs[guid] = path;
			m_filePaths[path] = guid;
			return;
		}
		m_assetGUIDs.emplace(guid, path);
		m_filePaths.emplace(path, guid);
	}

	bool AssetMapper::GetGUID(const std::filesystem::path& path, GUID& guid)
	{
		const auto& outputPath = m_filePaths.find(path);
		if (outputPath != m_filePaths.end())
		{
			guid = outputPath->second;
			return true;
		}
		return false;
	}

	void AssetMapper::BeginLoad()
	{
#ifdef MAPPER_LOAD_ASYNC
		JobManager::Add<LoadAssetMapperJob>(*this);
#else
		DoLoad();
#endif
	}

	void AssetMapper::DoLoad()
	{
		PROFILE_SCOPE(DoLoad, AssetMapper);

		LoadFromPath(m_rootPath);
	}

	void AssetMapper::LoadFromPath(const std::filesystem::path& path)
	{
		if (std::filesystem::is_directory(path))
		{
			auto subdirectories = std::filesystem::directory_iterator(path);
			for (auto subpath : subdirectories)
			{
				LoadFromPath(subpath);
			}
			return;
		}

		if (path.has_extension())
		{
			std::filesystem::path extension = path.extension();
			auto metaFile = std::find(extension.begin(), extension.end(), L".meta");
			// Should ignore it if its a meta file.
			if (metaFile != extension.end())
			{
				return;
			}
			LoadAssetGUID(path);
		}
	}

	void AssetMapper::LoadAssetGUID(const std::filesystem::path& path)
	{
		std::filesystem::path metaFileExtension = path;
		metaFileExtension += L".meta";

		if (std::filesystem::exists(metaFileExtension))
		{
			JsonFileParser parser(metaFileExtension);
			if (parser.IsValid())
			{
				// Reads the GUID and saves it.
				rapidjson::Document& document = parser.GetDocument();
				rapidjson::Value& value = document.GetObject();
				uint64_t guid;
				ReadUint64(value, "GUID", guid);
				SetPath(guid, path);
			}
		}
		else
		{
			// Generates new json file and saves it.
			GUID guid;
			JsonFileWriter writer(metaFileExtension);
			writer.Write<uint64_t>("GUID", guid);
			writer.Flush();
			SetPath(guid, path);
		}
	}

	void AssetMapper::UnLoad()
	{
		m_assetGUIDs.clear();
		m_filePaths.clear();
	}
}