#pragma once

#include "GUID.h"
#include "Job.h"

#include <unordered_map>
#include <filesystem>
#include <mutex>

namespace Engine
{
	class AssetManager;

	class AssetMapper
	{
	public:
		AssetMapper(const std::filesystem::path& path)
			: m_assetGUIDs(1024), m_rootPath(path), m_filePaths(1024)
		{
		}

		std::filesystem::path GetPath(const GUID& guid);
		void SetPath(const GUID& guid, const std::filesystem::path& path);
		bool GetGUID(const std::filesystem::path& path, GUID& guid);

	private:
		void BeginLoad();
		void DoLoad();
		void UnLoad();

		void LoadFromPath(const std::filesystem::path& path);
		void LoadAssetGUID(const std::filesystem::path& path);

	private:
		std::unordered_map<GUID, std::filesystem::path> m_assetGUIDs;
		std::unordered_map<std::wstring, GUID> m_filePaths;
		std::filesystem::path m_rootPath;
		std::mutex m_mutex;

		friend class AssetManager;
		friend class LoadAssetMapperJob;
	};


	class LoadAssetMapperJob : public Job
	{
	public:
		LoadAssetMapperJob(AssetMapper& mapper)
			: m_assetMapper(std::ref<AssetMapper>(mapper)) { }

		void OnRun() override
		{
			m_assetMapper.DoLoad();
		}

	private:
		AssetMapper& m_assetMapper;
	};
}