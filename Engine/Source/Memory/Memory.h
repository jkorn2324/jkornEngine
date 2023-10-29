#pragma once

#include <memory>
#include <type_traits>

namespace Engine
{
	enum class MemoryEndianessType
	{
		Type_LittleEndian,
		Type_BigEndian,
		Type_Unknown
	};

	class Memory
	{
	public:
		static MemoryEndianessType GetEndianess();

		static void Memcpy(void* dst, const void* src, size_t size)
		{
			Memcpy(dst, const_cast<void*>(src), size);
		}

		static void Memcpy(void* dst, void* src, size_t size);
		static void Memset(void* dst, int32_t val, size_t size);

		template<typename T, typename... TArgs>
		static T* Alloc(TArgs&&... args)
		{
			return new T(std::forward<TArgs>(args)...);
		}

		template<typename T>
		static T* AllocArray(const size_t size)
		{
			return new T[size];
		}

		template<typename T>
		static void DeAlloc(T*& ptr)
		{
			delete ptr;
			ptr = nullptr;
		}

		template<typename T>
		static void DeAllocArray(T*& ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}

		static void* Malloc(const size_t size)
		{
			return std::malloc(size);
		}

		static void Free(void* ptr)
		{
			std::free(ptr);
		}

		template<typename T, typename... TArgs>
		static inline T* Construct(void* ptr, TArgs&&... args)
		{
			if (!ptr)
			{
				return nullptr;
			}
			// Placement New Constructor.
			return new (ptr) T(std::forward<TArgs>(args)...);
		}

		template<typename T>
		static inline void Destruct(T* ptr)
		{
			if (ptr)
			{
				ptr->~T();
			}
		}
	};
}
