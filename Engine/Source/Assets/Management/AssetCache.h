#pragma once

#include <unordered_map>
#include <memory>

namespace Engine
{
	class GUID;
	class Timestep;

	/// <summary>
	/// Asset Cache that maps TAsset to the TAssetID.
	/// </summary>
	template<typename TAsset, typename TAssetID = GUID>
	class AssetCache
	{
		typedef std::shared_ptr<TAsset> TAssetSharedPtr;

	public:
		AssetCache() { }
		~AssetCache()
		{
			m_assetCache.clear();
			m_unreferencedAssets.clear();
		}

		/// <summary>
		/// Gets the Asset ID from the shared asset ptr.
		/// </summary>
		bool GetAssetID(TAsset* asset, TAssetID& assetID) const
		{
			for (const auto& pair : m_assetCache)
			{
				const auto& pAssetID = pair->first;
				const auto& pAsset = pair->second;
				if (pAsset.get() == asset)
				{
					assetID = pAssetID;
					return true;
				}
			}
			return false;
		}

		/// <summary>
		/// Gets the Asset ID from the shared asset ptr.
		/// </summary>
		bool GetAssetID(const TAssetSharedPtr& asset, TAsset& assetID) const
		{
			for (const auto& pair : m_assetCache)
			{
				const auto& pAssetID = pair->first;
				const auto& pAsset = pair->second;
				if (pAsset.get() == asset.get())
				{
					assetID = pAssetID;
					return true;
				}
			}
			return false;
		}

		// Gets the Asset from the shared asset ptr;
		bool GetAsset(const TAssetID& assetID, TAssetSharedPtr& outPtr) const
		{
			const auto& found = m_assetCache.find(assetID);
			if (found != m_assetCache.end())
			{
				outPtr = found.second;
				return true;
			}
			return false;
		}

		bool ContainsAsset(const TAssetID& assetID) const
		{
			const auto& found = m_assetCache.find(assetID);
			return found != m_assetCache.end();
		}

		/// <summary>
		/// Refreshes the Asset Cache, removes dereferenced assets.
		/// </summary>
		void RefreshCache()
		{
			for (const auto& pair : m_assetCache)
			{
				int32_t referenceCount = pair->second.use_count();
				if (referenceCount <= 1)
				{
					m_unreferencedAssets.push_back(pair->first);
				}
			}
			while (m_unreferencedAssets.size() > 0)
			{
				int32_t lastAssetIndex = (int32_t)m_unreferencedAssets.size() - 1;
				const TAssetID& id = m_unreferencedAssets[lastAssetIndex];
				TAssetSharedPtr ptr = m_assetCache[id];
				m_assetCache.erase(id);
				m_unreferencedAssets.pop_back();
				ptr.reset();
			}
			m_unreferencedAssets.clear();
		}

		/// <summary>
		/// Caches the assets inside of the asset cache.
		/// </summary>
		bool Cache(const TAssetID& assetID, const TAssetSharedPtr& asset)
		{
			if (ContainsAsset(assetID)) return false;
			m_assetCache.emplace(assetID, ptr);
			return true;
		}

	private:
		std::unordered_map<TAssetID, TAssetSharedPtr> m_assetCache;
		std::vector<TAssetID> m_unreferencedAssets;
	};
}