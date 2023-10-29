#pragma once

#include "Cache.h"

namespace Engine
{
	/**
	 * The reference to the asset that we are referring to.
	 */
	template<typename TAsset>
	class AssetRef
	{
		/**
		 * ================================ BEGIN STATIC OPERATIONS ================================
		 */

	private:
		using TCachedAsset = typename TAsset*;
		static RuntimeCache<TCachedAsset> s_assetCache;
		static DefaultAllocator s_assetAllocator;

	public:

		/**
		 * Creates an asset.
		 */
		template<typename... TArgs>
		static AssetRef<TAsset> CreateAsset(TArgs&& ...args)
		{
			TAsset* assetCreated = s_assetAllocator.Allocate<TAsset, TArgs...>(std::forward<TArgs>(args)...);
			return AssetRef(s_assetCache.Cache(assetCreated));
		}

		/**
		 * ================================ END STATIC OPERATIONS =================================
		 */

	public:
		AssetRef()
			: m_runtimeID(-1) { }

		AssetRef(const AssetRef& assetRef)
			: m_runtimeID(assetRef.m_runtimeID) { }

		AssetRef(nullptr_t n)
			: m_runtimeID(-1) { }

	private:
		AssetRef(int32_t runtimeID)
			: m_runtimeID(runtimeID) { }

	public:
		AssetRef& operator=(const AssetRef& assetRef)
		{
			m_runtimeID = assetRef.m_runtimeID;
			return *this;
		}

		bool IsValid() const { return GetCachedAsset() != nullptr; }

		/**
		 * Determines whether or not the asset ref is valid. 
		 */
		operator bool() const
		{
			return IsValid();
		}

		/**
		 * DeAllocates/Releases the Asset.
		 */
		void Release()
		{
			auto asset = GetCachedAsset();
			if (!asset)
			{
				return;
			}
			// Uncaches the asset.
			s_assetCache.UnCache(m_runtimeID);
			s_assetAllocator.DeAllocate(asset);
		}

		/**
		 * The pointer to the asset. 
		 */
		TAsset* operator->() { return GetCachedAsset(); }

		/**
		 * The pointer to the asset. 
		 */
		TAsset* operator->() const { return GetCachedAsset(); }

		friend bool operator!=(const AssetRef& a, const AssetRef& b)
		{
			return a.m_runtimeID != b.m_runtimeID;
		}

		friend bool operator==(const AssetRef& a, const AssetRef& b)
		{
			return a.m_runtimeID == b.m_runtimeID;
		}

	private:
		TCachedAsset GetCachedAsset() const { return *(GetCachedAssetPtr()); }
		TCachedAsset* GetCachedAssetPtr() const { return s_assetCache.Get(m_runtimeID); }

	private:
		int32_t m_runtimeID;
	};

	template<typename TAsset>
	RuntimeCache<typename AssetRef<TAsset>::TCachedAsset> AssetRef<TAsset>::s_assetCache = RuntimeCache<typename AssetRef<TAsset>::TCachedAsset>(24, 16);
	template<typename TAsset>
	DefaultAllocator AssetRef<TAsset>::s_assetAllocator;
}