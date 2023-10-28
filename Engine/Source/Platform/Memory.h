#pragma once

#include <memory>

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

		template<typename T>
		static void Memcpy(T* dst, const T* src)
		{
			Memcpy(dst, const_cast<T*>(src));
		}
		template<typename T>
		static void Memcpy(T* dst, T* src)
		{
			Memcpy(reinterpret_cast<void*>(dst),
				reinterpret_cast<void*>(src), sizeof(T));
		}
		template<typename T>
		static void Memcpy(T* dst, const T* src, size_t size)
		{
			Memcpy(dst,
				const_cast<T*>(src), size);
		}
		template<typename T>
		static void Memcpy(T* dst, T* src, size_t size)
		{
			Memcpy(reinterpret_cast<void*>(dst), 
				reinterpret_cast<void*>(src), size);
		}
		static void Memcpy(void* dst, const void* src, size_t size)
		{
			Memcpy(dst, const_cast<void*>(src), size);
		}

		static void Memcpy(void* dst, void* src, size_t size);
		static void Memset(void* dst, int32_t val, size_t size);
	};
}
