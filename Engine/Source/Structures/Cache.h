#pragma once

#include "Memory.h"

namespace Engine
{
    // TODO: Template for when its cached

    template<typename T, size_t MaxCacheSize=32>
    class CompiledFixedCache
    {
    public:
        // The cache id.
        using CacheID = int32_t;
        
    private:
        using Byte = uint8_t;
        static constexpr size_t c_BitsCount = (MaxCacheSize / 8) + 1;
        
        /**
         * Gets the bit index.
         */
        static constexpr int8_t GetBitIndex(CacheID index)
        {
            return (index / 8);
        }
        
        /**
         * Gets the bit shift number.
         */
        static constexpr int8_t GetBit(CacheID index)
        {
            int8_t bitValue = 1 >> (index % 8);
            return bitValue;
        }

    public:
        CompiledFixedCache() = default;
        
        /**
         * Adds the cached value.
         */
        CacheID Cache(const T& cachedRef)
        {
            CacheID nextOpenCacheID = GetNextOpenCacheID();
            if (nextOpenCacheID == -1)
            {
                return -1;
            }
            SetCached(nextOpenCacheID, true);
            m_cache[nextOpenCacheID] = cachedRef;
            return nextOpenCacheID;
        }
        
        /**
         * Uncaches the value at the cache id.
         */
        void UnCache(CacheID cacheID)
        {
            // Uncaches the value at the cache id.
            SetCached(cacheID, false);
        }
        
        T* Get(CacheID cacheID) const
        {
            if (!IsCached(cacheID))
            {
                return nullptr;
            }
            return &m_cached[cacheID];
        }
        
        T* Get(CacheID cacheID)
        {
            if (!IsCached(cacheID))
            {
                return nullptr;
            }
            return &m_cached[cacheID];
        }
        
        /**
         * Determines whether or not its a valid cache id.
         */
        bool IsValidCacheID(const CacheID cacheID) const
        {
            return cacheID >= 0 && cacheID < MaxCacheSize;
        }
        
        /**
         * Determines whether or not it is cached.
         */
        bool IsCached(CacheID cacheID) const
        {
            if (!IsValidCacheID(cacheID))
            {
                return false;
            }
            const Byte& byte = m_bits[GetBitIndex(cacheID)];
            const int8_t bit = GetBit(cacheID);
            return (byte & bit) != 0;
        }
                
        CacheID GetNextOpenCacheID() const
        {
            for (auto curID = 0; curID < MaxCacheSize; ++curID)
            {
                if (!IsCached(curID))
                {
                    return curID;
                }
            }
            return -1;
        }
        
    private:
        /**
         * Sets the cached ID as cached.
         */
        void SetCached(CacheID cacheID, bool value)
        {
            if (!IsValidCacheID(cacheID))
            {
                return;
            }
            Byte& byteIndex = m_bits[GetBitIndex(cacheID)];
            int8_t bit = GetBit(cacheID);
            if (value)
            {
                byteIndex |= bit;
                return;
            }
            byteIndex = (byteIndex & ~bit);
        }

    private:
        T m_cache[MaxCacheSize];
        Byte m_bits[c_BitsCount];
    };
}
