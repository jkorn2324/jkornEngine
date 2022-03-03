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
			: m_assetGUIDs(), m_path(path)
		{
		}

		GUID GetGUID(const std::filesystem::path& path);
		std::filesystem::path GetPathFromGUID(const GUID& guid);

	private:
		void BeginLoad();
		void DoLoad();
		void UnLoad();

	private:
		std::unordered_map<std::wstring, GUID> m_assetGUIDs;
		std::wstring m_path;
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