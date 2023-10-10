#include "EnginePCH.h"
#include "AssetMapper.h"

#include "JobManager.h"
#include "JsonFileParser.h"
#include "JsonUtils.h"
#include "JsonFileWriter.h"
#include "Profiler.h"

#include <filesystem>

namespace Engine
{
    namespace
    {
        AssetMapper::PathIndex GetPathIndex(const std::filesystem::path& path)
        {
            const std::wstring wStr = path.wstring();
            return (AssetMapper::PathIndex)std::hash<std::wstring>()(wStr);
        }
    }

	void AssetMapper::RemovePath(const GUID& guid)
	{
		GUIDIndex guidIndex = std::hash<GUID>()(guid);
		const auto& foundPath = m_guidToPaths.find(guidIndex);
		if (foundPath != m_guidToPaths.end())
		{
			PathIndex pathIndex = GetPathIndex(foundPath->second);
			const auto& foundGUID = m_pathToGUIDs.find(pathIndex);
			if (foundGUID != m_pathToGUIDs.end())
			{
				m_pathToGUIDs.erase(foundGUID);
			}
			m_guidToPaths.erase(foundPath);
		}
	}	

	void AssetMapper::RemovePath(const std::filesystem::path& path)
	{
        PathIndex pathIndex = GetPathIndex(path);
		const auto& foundGUID = m_pathToGUIDs.find(pathIndex);
		if (foundGUID != m_pathToGUIDs.end())
		{
			const auto& guid = foundGUID->second;
			GUIDIndex guidIndex = std::hash<GUID>()(guid);
			const auto& foundPath = m_guidToPaths.find(guidIndex);
			if (foundPath != m_guidToPaths.end())
			{
				m_guidToPaths.erase(foundPath);
			}
			m_pathToGUIDs.erase(foundGUID);
		}
	}

	void AssetMapper::SetPath(const GUID& guid, const std::filesystem::path& path)
	{
		// Adds it to the guids to path unordered map.
		GUIDIndex guidIndex = std::hash<GUID>()(guid);
		const auto& foundPath = m_guidToPaths.find(guidIndex);
		if (foundPath != m_guidToPaths.end())
		{
			m_guidToPaths[guidIndex] = path;
		}
		else
		{
			m_guidToPaths.emplace(guidIndex, path);
		}

		// Adds it the path to guids unordered map.
        PathIndex pathIndex = GetPathIndex(path);
		const auto& foundGUID = m_pathToGUIDs.find(pathIndex);
		if (foundGUID != m_pathToGUIDs.end())
		{
			m_pathToGUIDs[pathIndex] = guid;
		}
		else
		{
			m_pathToGUIDs.emplace(pathIndex, guid);
		}
	}

	bool AssetMapper::GetPath(const GUID& guid, std::filesystem::path& path) const
	{
		GUIDIndex guidHash = std::hash<GUID>()(guid);
		const auto& outputPath = m_guidToPaths.find(guidHash);
		if (outputPath != m_guidToPaths.end())
		{
			path = outputPath->second;
			return true;
		}
		return false;
	}

	bool AssetMapper::GetGUID(const std::filesystem::path& path, GUID& guid) const
	{
        PathIndex pathHash = GetPathIndex(path);
		const auto& outputGUID = m_pathToGUIDs.find(pathHash);
		if (outputGUID != m_pathToGUIDs.end())
		{
			guid = outputGUID->second;
			return true;
		}
		return false;
	}

	void AssetMapper::Clear()
	{
		m_pathToGUIDs.clear();
		m_guidToPaths.clear();
	}
}
