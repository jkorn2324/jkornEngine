#pragma once

#include "Allocator.h"
#include "Memory.h"

#include <cstdint>

namespace Engine
{
    // TODO: Template for when its cached
    namespace CacheUtilities
    {
        static inline constexpr size_t GetBitsCount(size_t cacheSize)
        {
            return (cacheSize / 8) + 1;
        }

        template<typename T = int32_t>
        static inline constexpr size_t GetBitIndex(T index)
        {
            return index % 8;
        }


        /**
         * Gets the bit shift number.
         */
        template<typename T = int32_t>
        static inline constexpr int8_t GetBit(T index)
        {
            int8_t bitValue = 1 >> (index % 8);
            return bitValue;
        }
    }

    template<typename T, typename TAllocator = DefaultAllocator>
    class RuntimeCache
    {
        static_assert(IsAllocator<TAllocator>::Value, "The allocator is not a valid allocator.");

    public:
        using CacheID = int32_t;

    private:
        using Byte = uint8_t;

    public:
        RuntimeCache(size_t capacity, size_t resizeAmount)
            : m_cache(nullptr), m_bits(nullptr), m_allocator(), m_capacity(capacity), m_resizeAmount(resizeAmount)
        {
            Allocate(capacity);
        }

        ~RuntimeCache()
        {
            DeAllocate();
        }

    private:
        void Allocate(size_t capacity)
        {
            m_cache = m_allocator.Allocate<T>(capacity);

            const size_t bitsCount = CacheUtilities::GetBitsCount(capacity);
            m_bits = m_allocator.Allocate<Byte>(bitsCount);
            Memory::Memset(m_bits, 0, bitsCount);
        }

        void DeAllocate()
        {
            DeAllocate(m_cache, m_bits, m_capacity);
        }

        void DeAllocate(T*& cache, Byte*& bits, const size_t capacity)
        {
            m_allocator.DeAllocate<T>(cache, capacity);
            m_allocator.DeAllocate<Byte>(bits, CacheUtilities::GetBitsCount(capacity));
        }

    public:
        /**
         * Adds the cached value.
         */
        CacheID Cache(const T& cachedRef)
        {
            CacheID nextOpenCacheID = GetNextOpenCacheID();
            if (nextOpenCacheID == -1)
            {
                // Updates the capacity.
                size_t prevLength = m_capacity;
                m_capacity += m_resizeAmount;

                // Resizes the buffer.
                {
                    // Caches & stores cache + bits.
                    T* prevCache = m_cache;
                    Byte* prevBits = m_bits;

                    Allocate(m_capacity);
                    Memory::Memcpy(m_cache, prevCache, sizeof(T) * prevLength);
                    Memory::Memcpy(m_bits, prevBits, sizeof(Byte) * CacheUtilities::GetBitsCount(prevLength));
                    DeAllocate(prevCache, prevBits, prevLength);
                }
                nextOpenCacheID = (CacheID)prevLength;
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
            if constexpr (std::is_pointer<T>::value)
            {
                bool updateCache = SetCached(cacheID, false);
                if (updateCache)
                {
                    m_cache[cacheID] = nullptr;
                }
                return;
            }
            SetCached(cacheID, false);
        }

        size_t GetCapacity() const { return m_capacity; }

        decltype(auto) operator[](CacheID cacheID) const
        {
            return Get(cacheID);
        }

        decltype(auto) Get(CacheID cacheID) const
        {
            // The cache is a T* Pointer.
            if (IsCached(cacheID))
            {
                T& cache = m_cache[cacheID];
                return (T*)&cache;
            }
            return (T*)nullptr;
        }

        /**
         * Determines whether or not its a valid cache id.
         */
        bool IsValidCacheID(const CacheID cacheID) const
        {
            return cacheID >= 0 && cacheID < m_capacity;
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
            const Byte& byte = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            const int8_t bit = CacheUtilities::GetBit(cacheID);
            return (byte & bit) != 0;
        }

        CacheID GetNextOpenCacheID() const
        {
            for (auto curID = 0; curID < m_capacity; ++curID)
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
        bool SetCached(CacheID cacheID, bool value)
        {
            if (!IsValidCacheID(cacheID))
            {
                return false;
            }
            Byte& byteIndex = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            int8_t bit = CacheUtilities::GetBit(cacheID);
            if (value)
            {
                byteIndex |= bit;
                return true;
            }
            byteIndex = (byteIndex & ~bit);
            return true;
        }

    private:
        T* m_cache;
        Byte* m_bits;
        TAllocator m_allocator;
        size_t m_capacity;
        size_t m_resizeAmount;
    };

    template<typename T, typename TAllocator = DefaultAllocator>
    class RuntimeFixedCache
    {
        // Statically asserts that we have a valid allocator.
        static_assert(IsAllocator<TAllocator>::Value, "The allocator is not a valid allocator.");

    public:
        using CacheID = int32_t;

    private:
        using Byte = uint8_t;

    public:
        RuntimeFixedCache(size_t capacity)
            : m_cache(nullptr), m_bits(nullptr), m_allocator(), m_capacity(capacity)
        {
            Allocate(capacity);
        }

        ~RuntimeFixedCache()
        {
            DeAllocate();
        }

    private:
        void Allocate(size_t capacity)
        {
            m_cache = m_allocator.Allocate<T>(capacity);
            m_bits = m_allocator.Allocate<Byte>(CacheUtilities::GetBitsCount(capacity));
        }

        void DeAllocate()
        {
            m_allocator.DeAllocate<T>(m_cache, m_capacity);
            m_allocator.DeAllocate<Byte>(m_bits, CacheUtilities::GetBitsCount(m_capacity));
        }

    public:
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
            if constexpr (std::is_pointer<T>::value)
            {
                bool updateCache = SetCached(cacheID, false);
                if (updateCache)
                {
                    m_cache[cacheID] = nullptr;
                }
                return;
            }
            SetCached(cacheID, false);
        }

        size_t GetCapacity() const { return m_capacity; }

        decltype(auto) operator[](CacheID cacheID) const
        {
            return Get(cacheID);
        }

        decltype(auto) Get(CacheID cacheID) const
        {
            // The cache is a T* Pointer.
            if (IsCached(cacheID))
            {
                T& cache = m_cache[cacheID];
                return (T*)&cache;
            }
            return (T*)nullptr;
        }

        /**
         * Determines whether or not its a valid cache id.
         */
        bool IsValidCacheID(const CacheID cacheID) const
        {
            return cacheID >= 0 && cacheID < m_capacity;
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
            const Byte& byte = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            const int8_t bit = CacheUtilities::GetBit(cacheID);
            return (byte & bit) != 0;
        }

        CacheID GetNextOpenCacheID() const
        {
            for (auto curID = 0; curID < m_capacity; ++curID)
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
            Byte& byteIndex = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            int8_t bit = CacheUtilities::GetBit(cacheID);
            if (value)
            {
                byteIndex |= bit;
                return;
            }
            byteIndex = (byteIndex & ~bit);
        }

    private:
        T* m_cache;
        Byte* m_bits;
        TAllocator m_allocator;
        size_t m_capacity;

    };

    template<typename T, size_t MaxCacheSize=32>
    class CompiledFixedCache
    {
    public:
        // The cache id.
        using CacheID = int32_t;
        
    private:
        using Byte = uint8_t;
        static constexpr size_t c_BitsCount = CacheUtilities::GetBitsCount(MaxCacheSize);
        
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
            if constexpr (std::is_pointer<T>::value)
            {
                bool updateCache = SetCached(cacheID, false);
                if (updateCache)
                {
                    m_cache[cacheID] = nullptr;
                }
                return;
            }
            SetCached(cacheID, false);
        }

        decltype(auto) operator[](CacheID cacheID) const
        {
            return Get(cacheID);
        }
        
        decltype(auto) Get(CacheID cacheID) const
        {
            // The cache is a T* Pointer.
            if (IsCached(cacheID))
            {
                T& cache = m_cache[cacheID];
                return (T*)&cache;
            }
            return (T*)nullptr;
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
            const Byte& byte = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            const int8_t bit = CacheUtilities::GetBit(cacheID);
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
            Byte& byteIndex = m_bits[CacheUtilities::GetBitIndex(cacheID)];
            int8_t bit = CacheUtilities::GetBit(cacheID);
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
