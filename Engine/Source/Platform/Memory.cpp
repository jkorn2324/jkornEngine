#include "EnginePCH.h"
#include "Memory.h"

#include <memory>

namespace Engine
{
	constexpr MemoryEndianessType Memory::GetEndianess()
	{
		uint32_t endian = 1;
		uint8_t* ptr = (uint8_t*)&endian;
		if (*ptr == 1) return MemoryEndianessType::Type_LittleEndian;
		return MemoryEndianessType::Type_BigEndian;
	}

	void Memory::Memcpy(void* dst, void* src, size_t size)
	{
		std::memcpy(dst, src, size);
	}

	void Memory::Memset(void* dst, int32_t val, size_t size)
	{
		std::memset(dst, val, size);
	}
}