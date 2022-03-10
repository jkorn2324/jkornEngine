#include "EnginePCH.h"
#include "AssetMapper.h"

#include "JobManager.h"
#include "JsonFileParser.h"
#include "JsonUtils.h"

namespace Engine
{
	std::filesystem::path AssetMapper::GetPath(const GUID& guid)
	{
		m_mutex.lock();
		const auto& outputPath = m_assetGUIDs.find(guid);
		if(outputPath != m_assetGUIDs.end())
		{
			m_mutex.unlock();
			return outputPath->second;
		}
		m_mutex.unlock();
		return std::filesystem::path();
	}
	
	void AssetMapper::SetPath(const GUID& guid, const std::filesystem::path& path)
	{
		m_mutex.lock();
		const auto& outputPath = m_assetGUIDs.find(guid);
		if (outputPath != m_assetGUIDs.end())
		{
			m_assetGUIDs[guid] = path;
			m_mutex.unlock();
			return;
		}
		m_assetGUIDs.emplace(guid, path);
		m_mutex.unlock();
	}

	void AssetMapper::BeginLoad()
	{
		JobManager::Add<LoadAssetMapperJob>(*this);
	}

	void AssetMapper::DoLoad()
	{
		m_mutex.lock();

		{
			JsonFileParser fileParser(m_path);
			if (!fileParser.IsValid())
			{
				m_mutex.unlock();
				return;
			}
			// TODO: Load the Asset GUIDs.
		}

		m_mutex.unlock();
	}

	void AssetMapper::UnLoad()
	{
		m_mutex.lock();
		
		{
			rapidjson::PrettyWriter<rapidjson::StringBuffer> jsonWriter;


			for (const auto& pair : m_assetGUIDs)
			{

			}
		}

		m_mutex.unlock();
	}
}