#include "EnginePCH.h"
#include "AssetMapper.h"

#include "JobManager.h"
#include <sstream>

namespace Engine
{
	GUID AssetMapper::GetGUID(const std::filesystem::path& path)
	{
		m_mutex.lock();
		const auto& found = m_assetGUIDs.find(path);
		if (found != m_assetGUIDs.end())
		{
			m_mutex.unlock();
			return found->second;
		}
		GUID guid;
		m_assetGUIDs.emplace(path, guid);
		m_mutex.unlock();
		return guid;
	}

	// File Path should be {path}/{guid[0-2]}/guid/
	std::filesystem::path AssetMapper::GetPathFromGUID(const GUID& guid)
	{
		// TODO: Implementation
		return "";
	}
	
	void AssetMapper::BeginLoad()
	{
		JobManager::Add<LoadAssetMapperJob>(*this);
	}

	void AssetMapper::DoLoad()
	{
		m_mutex.lock();

		// TODO: Read the Asset GUIDs.
		{

		}

		m_mutex.unlock();
	}

	void AssetMapper::UnLoad()
	{
		m_mutex.lock();

		// TODO: Unload the Asset GUIDs.
		{

		}

		m_mutex.unlock();
	}
}