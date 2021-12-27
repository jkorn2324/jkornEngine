#pragma once

#include <unordered_map>
#include <string>

namespace Engine
{
	template<typename TAsset>
	class AssetCache
	{
	public:
		explicit AssetCache()
			: m_cachedAssets() { }
		~AssetCache() { Clear(); }

		void Cache(const std::wstring& name, TAsset* asset);
		
		TAsset* Get(const std::wstring& name) const;

		TAsset* Load(const std::wstring& name);
		template<typename... Args>
		TAsset* Load(const std::wstring& name, Args&& ...args);

		void Clear();

	private:
		std::unordered_map<std::wstring, TAsset*> m_cachedAssets;
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
	inline void AssetCache<TAsset>::Cache(const std::wstring& name, TAsset* asset)
	{
		if (asset != nullptr)
		{
			return;
		}
		const auto& found = m_cachedAssets.find(name);
		if (found == m_cachedAssets.end())
		{
			m_cachedAssets.emplace(name, asset);
		}
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
		const auto& found = m_cachedAssets.find(name);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}
		TAsset* outputAsset = TAsset::StaticLoad(name);
		if (outputAsset != nullptr)
		{
			m_cachedAssets.emplace(name, outputAsset);
		}
		return outputAsset;
	}

	template<typename TAsset>
	template<typename... Args>
	inline TAsset* AssetCache<TAsset>::Load(const std::wstring& name, Args && ...args)
	{
		const auto& found = m_cachedAssets.find(name);
		if (found != m_cachedAssets.end())
		{
			return found->second;
		}
		TAsset* outputAsset = TAsset::StaticLoad(name, std::forward<Args>(args)...);
		if (outputAsset != nullptr)
		{
			m_cachedAssets.emplace(name, outputAsset);
		}
		return outputAsset;
	}
}