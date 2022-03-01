#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <tuple>
#include <mutex>

#include "Job.h"
#include "JobManager.h"

namespace Engine
{

	template<typename TAsset>
	class AssetCache
	{
#pragma region jobs

	public:
		class AsyncLoadAssetNoArgsJob : public Job
		{
		public:
			AsyncLoadAssetNoArgsJob(const std::wstring& path, AssetCache<TAsset>& assetCache)
				: m_assetCache(assetCache), m_path(path)
			{

			}

			void OnRun() override
			{
				m_mutex.lock();
				m_assetCache.Load(m_path);
				m_mutex.unlock();
			}

		private:
			AssetCache<TAsset>& m_assetCache;
			std::wstring m_path;
			std::mutex m_mutex;
		};

	public:
		template<typename ...Args>
		class AsyncLoadAssetArgsJob : public Job
		{
			typedef std::tuple<Args...> JobArgs;

		public:
			AsyncLoadAssetArgsJob(const std::wstring& path, AssetCache<TAsset>& assetCache, Args... args)
				: m_path(path), m_assetCache(assetCache), m_args(std::forward<Args>(args)...)
			{
			}


			void OnRun() override
			{
				LoadAsset(std::index_sequence_for<Args...>());
			}

		private:
			template<std::size_t... Sq>
			void LoadAsset(std::index_sequence<Sq...>)
			{
				m_assetCache.Load(m_path, std::get<Sq>(m_args)...);
			}

		private:
			AssetCache<TAsset>& m_assetCache;
			JobArgs m_args;
			std::wstring m_path;
		};

#pragma endregion

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

		void AsyncLoad(const std::wstring& name);
		template<typename...Args>
		void AsyncLoad(const std::wstring& name, Args ...args);

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
		TAsset* asset = new TAsset();
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
		TAsset* created = new TAsset(std::forward<Args>(args)...);
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
		TAsset* outputAsset = TAsset::StaticLoad(name);
		if (outputAsset != nullptr)
		{
			m_cachedAssets.emplace(name, outputAsset);
		}
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
		TAsset* outputAsset = TAsset::StaticLoad(name, std::forward<Args>(args)...);
		if (outputAsset != nullptr)
		{
			m_cachedAssets.emplace(name, outputAsset);
		}
		m_mutex.unlock();
		return outputAsset;
	}

	template<typename TAsset>
	inline void AssetCache<TAsset>::AsyncLoad(const std::wstring& name)
	{
		if (m_supportsMultithreading)
		{
			JobManager::Add<AssetCache<TAsset>::AsyncLoadAssetNoArgsJob>(name, *this);
		}
		else
		{
			Load(name);
		}
	}

	// Args are copied so that they can be added to a tuple and read on another thread.

	template<typename TAsset>
	template<typename... Args>
	inline void AssetCache<TAsset>::AsyncLoad(const std::wstring& name, Args... args)
	{
		typedef AssetCache<TAsset>::AsyncLoadAssetArgsJob<Args...> JobType;

		if (m_supportsMultithreading)
		{
			JobManager::Add<JobType>(name, *this, args...);
		}
		else
		{
			Load(name, std::forward<Args>(args)...);
		}
	}
}