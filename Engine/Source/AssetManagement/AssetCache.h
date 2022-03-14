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
		std::mutex m_mutex;
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
		m_mutex.lock();
		TAsset* found = Get(guid);
		if (found != nullptr)
		{
			m_mutex.unlock();
			return found;
		}
		TAsset* asset = TAsset::Create();
		if (asset == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}
		m_cachedAssets.emplace(guid, asset);
		m_mutex.unlock();
		return asset;
	}

	template<typename TAsset>
	template<typename ...Args>
	inline TAsset* AssetCache<TAsset>::Cache(const GUID& guid, Args && ...args)
	{
		m_mutex.lock();
		TAsset* found = Get(guid);
		if (found != nullptr)
		{
			m_mutex.unlock();
			return found;
		}
		TAsset* created = TAsset::Create(std::forward<Args>(args)...);
		if (created == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}
		m_cachedAssets.emplace(guid, created);
		m_mutex.unlock();
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
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			m_mutex.unlock();
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromGUID(guid))
		{
			m_mutex.unlock();
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, const std::filesystem::path& path)
	{
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			m_mutex.unlock();
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromFile(path))
		{
			m_mutex.unlock();
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	template<typename... Args>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, Args && ...args)
	{
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			m_mutex.unlock();
			return found->second;
		}
		
		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromGUID(guid, std::forward<Args>(args)...))
		{
			m_mutex.unlock();
			delete outputAsset;
			return false;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	template<typename...Args>
	inline TAsset* AssetCache<TAsset>::Load(const GUID& guid, const std::filesystem::path& path, Args&&...args)
	{
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			m_mutex.unlock();
			return found->second;
		}

		TAsset* outputAsset = TAsset::Create();
		if (outputAsset == nullptr)
		{
			m_mutex.unlock();
			return nullptr;
		}

		AssetSerializer<TAsset> assetSerializer(*outputAsset);
		if (!assetSerializer.DeserializeFromFile(path, std::forward<Args>(args)...))
		{
			m_mutex.unlock();
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(guid, outputAsset);
		m_mutex.unlock();
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
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(guid);
		if (found != m_cachedAssets.end())
		{
			// TODO: If we save, than deserialize the asset
			delete found->second;
			m_cachedAssets.erase(found);
			m_mutex.unlock();
			return;
		}
		m_mutex.unlock();
	}
}