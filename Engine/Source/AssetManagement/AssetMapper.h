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
		typedef size_t PathIndex;
		typedef size_t GUIDIndex;

	public:
		AssetMapper(const std::filesystem::path& path)
			: m_pathToGUIDs(1024), m_rootPath(path), m_guidToPaths(1024)
		{
		}

		bool GetPath(const GUID& guid, std::filesystem::path& path) const;
		bool GetGUID(const std::filesystem::path& path, GUID& guid) const;

		void SetPath(const GUID& guid, const std::filesystem::path& path);

	private:
		// Used to remove the asset when it isn't being used anymore.
		void RemovePath(const GUID& guid);
		void RemovePath(const std::filesystem::path& path);

		void Clear();

	private:
		std::unordered_map<PathIndex, GUID> m_pathToGUIDs;
		std::unordered_map<GUIDIndex, std::filesystem::path> m_guidToPaths;
		std::filesystem::path m_rootPath;

		friend class AssetManager;
		friend class LoadAssetMapperJob;
	};
}
