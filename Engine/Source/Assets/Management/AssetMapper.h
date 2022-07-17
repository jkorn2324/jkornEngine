#pragma once

#include <unordered_map>
#include <filesystem>

namespace Engine
{
	class GUID;

	template<typename TAssetID = GUID>
	class AssetMapper
	{
		using Filesystem = std::filesystem;
		using FilePath = std::filesystem::path;

	public:
		AssetMapper() { }

		template<typename TDeserializer>
		void LoadPaths(const FilePath& filePath)
		{ 
			// TODO: Implementation
		}

		template<typename TSerializer>
		void ExportPaths(const FilePath& filePath)
		{
			// TODO: Implementation
		}

		void SetPath(const FilePath& path, const TAssetID& assetID)
		{
			if (ContainsFilePath(path)) return;
			m_pathsToAssetIDs.emplace(path, assetID);
		}

		bool ContainsFilePath(const FilePath& path) const
		{
			const auto& found = m_pathsToAssetIDs.find(path);
			return found != m_pathsToAssetIDs.end();
		}

		bool GetAssetID(const FilePath& path, TAssetID& assetID) const
		{
			const auto& found = m_pathsToAssetIDs.find(path);
			if (found != m_pathsToAssetIDs.end())
			{
				assetID = found->second;
				return true;
			}
			return false;
		}

	private:
		std::unordered_map<FilePath, TAssetID> m_pathsToAssetIDs;
	};
}