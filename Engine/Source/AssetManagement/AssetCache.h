#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <tuple>
#include <mutex>

#include "Job.h"
#include "JobManager.h"
#include "AssetSerializer.h"

namespace Engine
{

	// Responsible for loading and unloading assets.
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
		TAsset* Cache(const std::wstring& name, Args&& ...args);
		TAsset* Cache(const std::wstring& name);
		TAsset* Get(const std::wstring& name) const;

		TAsset* Load(const std::wstring& name);
		template<typename... Args>
		TAsset* Load(const std::wstring& name, Args&& ...args);

		void Unload(const std::wstring& name);
		void Unload(const std::wstring& name, bool save);

		void Clear();

	private:
		std::unordered_map<std::wstring, TAsset*> m_cachedAssets;
		std::mutex m_mutex;
		bool m_supportsMultithreading;
	};

	template<typename TAsset>
	inline TAsset* AssetCache<TAsset>::Get(const std::wstring& name) const
	{
		const auto& found = m_cachedAssets.find(name);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}
		return nullptr;
	}

	template<typename TAsset>
	TAsset* AssetCache<TAsset>::Cache(const std::wstring& name)
	{
		m_mutex.lock();
		TAsset* found = Get(name);
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
		m_cachedAssets.emplace(name, asset);
		m_mutex.unlock();
		return asset;
	}

	template<typename TAsset>
	template<typename ...Args>
	inline TAsset* AssetCache<TAsset>::Cache(const std::wstring& name, Args && ...args)
	{
		m_mutex.lock();
		TAsset* found = Get(name);
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
		m_cachedAssets.emplace(name, created);
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
	inline TAsset* AssetCache<TAsset>::Load(const std::wstring& name)
	{
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(name);
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
		if (!assetSerializer.DeserializeFromFile(name))
		{
			m_mutex.unlock();
			delete outputAsset;
			return nullptr;
		}
		m_cachedAssets.emplace(name, outputAsset);
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	template<typename... Args>
	inline TAsset* AssetCache<TAsset>::Load(const std::wstring& name, Args && ...args)
	{
		m_mutex.lock();
		const auto& found = m_cachedAssets.find(name);
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
		if (!assetSerializer.DeserializeFromFile(name, std::forward<Args>(args)...))
		{
			m_mutex.unlock();
			delete outputAsset;
			return false;
		}
		m_cachedAssets.emplace(name, outputAsset);
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::Unload(const std::wstring& name)
	{
		Unload(name, false);
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::Unload(const std::wstring& name, bool save)
	{
		// TODO: Implementation
	}
}