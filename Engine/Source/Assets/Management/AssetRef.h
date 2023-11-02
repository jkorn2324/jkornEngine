#pragma once

#include "Asset.h"
#include "Cache.h"
#include "Allocator.h"

namespace Engine
{

    namespace Internals::Assets
    {

        template<typename TAsset, typename TAllocator, size_t InitialAssets, size_t ResizeAmount>
        class TAssetManager
        {
			static_assert(IsAsset<TAsset>::Value, "The asset must be a valid asset.");
            static_assert(IsAllocator<TAllocator>::Value, "The allocator must be a valid allocator.");
            
        public:
            using Cache = typename RuntimeCache<TAsset*, TAllocator>;
            using CachedAssetID = typename Cache::CacheID;
            
        public:
            TAssetManager() : m_runtimeCache(InitialAssets, ResizeAmount), m_allocator() { }
            
            /**
             * Clears the assets.
             */
            void Clear()
            {
                const size_t capacity = m_runtimeCache.GetCapacity();
                for (size_t i = 0; i < capacity; ++i)
                {
                    CachedAssetID assetID = (CachedAssetID)i;
                    Remove(assetID);
                }
            }
            
            template<typename ... TArgs>
            CachedAssetID Create(TArgs&&... args)
            {
                TAsset* assetCreated = m_allocator.typename Allocate<TAsset, TArgs...>(std::forward<TArgs>(args)...);
                CachedAssetID cachedAssetID = m_cache.Cache(assetCreated);
                return cachedAssetID;
            }
            
            void Remove(CachedAssetID cachedAssetID)
            {
                if (!m_cache.IsCached(cachedAssetID))
                {
                    return;
                }
                // Uncaches the asset.
                TAsset* asset = m_cache[cachedAssetID];
                m_cache.UnCache(cachedAssetID);
                m_allocator.typename DeAllocate<TAsset>(asset);
            }
            
            /**
             * Gets the asset from the cached asset id.
             */
            TAsset* Get(CachedAssetID cachedAssetID) const
            {
                return m_cache[cachedAssetID];
            }
            
            /**
             * Searches through the asset cache for the "right" asset.
             * The func callback has the following structure: (CachedAssetID id, const TContext& context) -> bool;
             */
            template<typename TContext, typename TFunc>
            CachedAssetID Find(const TContext& context, const TFunc& callback) const
            {
                // If there isn't a callback, than we aren't doing anything.
                if (!callback)
                {
                    return -1;
                }
                size_t capacity = m_cache.GetCapacity();
                for (size_t i = 0; i < capacity; ++i)
                {
                    CachedAssetID cacheID = (CachedAssetID)i;
                    // If we are cached and the callback is valid, this is
                    // the one we are looking for.
                    if (m_cache.IsCached(cacheID)
                        && callback(cacheID, context))
                    {
                        return cacheID;
                    }
                }
                return -1;
            }
            
        private:
            Cache m_cache;
            TAllocator m_allocator;
        };
    }
	/**
	 * The reference to the asset that we are referring to.
	 */
	template<typename TAsset>
	class AssetRef
	{
        /**
         * ================================ BEGIN STATIC OPERATIONS ==================================
         */
        
    private:
        static Internals::Assets::TAssetManager<TAsset*, DefaultAllocator, 32, 16> s_assetManager;
	
    public:

		/**
		 * Creates an asset.
		 */
		template<typename... TArgs>
		static AssetRef<TAsset> CreateAsset(TArgs&& ...args)
		{
            return s_assetManager.Create(std::forward<TArgs>(args)...);
		}
        
        /**
         * Finds an asset from the context & based on a callback.
         */
        template<typename TContext, typename TFunc>
        static AssetRef<TAsset> FindAsset(const TContext& context, const TFunc& callback) const
        {
            return AssetRef<TAsset>(s_assetManager.Find(context, callback));
        }
        
        /**
         * Clears the assets in the typed asset manager.
         */
        static void ClearAssets()
        {
            s_assetManager.Clear();
        }

        /**
		 * ================================ END STATIC OPERATIONS ==================================
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
            s_assetManager.Remove(m_runtimeID);
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
		TAsset* GetCachedAsset() const { return *(GetCachedAssetPtr()); }
		TAsset** GetCachedAssetPtr() const { return s_assetManager.Get(m_runtimeID); }

	private:
		int32_t m_runtimeID;
	};
}
