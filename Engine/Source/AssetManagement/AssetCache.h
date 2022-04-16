#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <tuple>
#include <mutex>

#include "Job.h"
#include "JobManager.h"
#include "AssetSerializer.h"
#include "GUID.h"

namespace Engine
{

	// Responsible for loading and unloading assets with GUIDs as 
	// key & pointers as their values.
	// This doesn't determine how many times the asset is loaded;
	// that is determined by the reference manager.
	template<typename TAsset>
	class AssetCache
	{
	public:
		explicit AssetCache()
			: m_cachedAssets(), m_supportsMultithreading(true) { }
		explicit AssetCache(bool supportsMultithreading)
			: m_cachedAssets(), m_supportsMultithreading(supportsMultithreading) { }
		~AssetCache() { Clear(); }

		template<typename... Args>
		TAsset* Cache(const GUID& guid, Args&& ...args);
		TAsset* Cache(const GUID& guid);
		TAsset* Get(const GUID& guid) const;

		TAsset* Load(const GUID& guid);
		TAsset* Load(const GUID& guid, const std::filesystem::path& path);
		template<typename... Args>
		TAsset* Load(const GUID& guid, Args&& ...args);
		template<typename... Args>
		TAsset* Load(const GUID& guid, const std::filesystem::path& path, Args&&...args);

		void Unload(const GUID& guid);
		void Unload(const GUID& guid, bool save);

		void Clear();
		
	private:
		std::unordered_map<GUID, TAsset*> m_cachedAssets;
		bool m_supportsMultithreading;
	};

	template<typename TAsset>
	inline TAsset* AssetCache<TAsset>::Get(const GUID& guid) const
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}
		return nullptr;
	}

	template<typename TAsset>
	TAsset* AssetCache<TAsset>::Cache(const GUID& guid)
	{
		TAsset* found = Get(guid);
		if (found != nullptr)
		{
			return found;
		}
		TAsset* asset = TAsset::Create();
		if (asset == nullptr)
		{
			return nullptr;
		}
		m_cachedAssets.emplace(guid, asset);
		return asset;
	}

	template<typename TAsset>
	template<typename ...Args>
	inline TAsset* AssetCache<TAsset>::Cache(const GUID& guid, Args && ...args)
	{
		TAsset* found = Get(guid);
		if (found != nullptr)
		{
			return found;
		}
		TAsset* created = TAsset::Create(std::forward<Args>(args)...);
		if (created == nullptr)
		{
			return nullptr;
		}
		m_cachedAssets.emplace(guid, created);
		return created;
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::Clear()
	{
		for (auto pair : m_cachedAssets)
		{
			delete pair.second;
		}
		m_cachedAssets.clear();
	}

	template<typename TAsset>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid)
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromGUID(guid))
		{
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		return outputAsset;
	}

	template<typename TAsset>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, const std::filesystem::path& path)
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromFile(path))
		{
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		return outputAsset;
	}

	template<typename TAsset>
	template<typename... Args>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, Args && ...args)
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}
		
		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromGUID(guid, std::forward<Args>(args)...))
		{
			delete outputAsset;
			return false;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		return outputAsset;
	}

	template<typename TAsset>
	template<typename...Args>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, const std::filesystem::path& path, Args&&...args)
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromFile(path, std::forward<Args>(args)...))
		{
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		return outputAsset;
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::Unload(const GUID& guid)
	{
		Unload(guid, false);
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::Unload(const GUID& guid, bool save)
	{
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			// TODO: If we save, than deserialize the asset
			delete found->second;
			m_cachedAssets.erase(found);
			return;
		}
	}
}