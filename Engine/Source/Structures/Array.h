#pragma once

#include "Buffer.h"

namespace Engine
{

	template<typename T, size_t Capacity>
	class FixedArray
	{
	public:
		static constexpr size_t FixedBufferSize = Capacity * sizeof(T);
		using Buffer = FixedBuffer<FixedBufferSize>;

	public:
		const Buffer& GetBuffer() const { return m_buffer; }

		constexpr size_t GetCapacity() const { return Capacity; }

		T& operator[](size_t index) const
		{
			JKORN_ENGINE_ASSERT(IsValidIndex(index), "The index must be inside of the buffer.");
			return *m_buffer.GetViaIndex<T>(index);
		}

		T& operator[](size_t index)
		{
			JKORN_ENGINE_ASSERT(IsValidIndex(index), "The index must be valid.");
			T* buffer = reinterpret_cast<T*>(m_buffer.GetBuffer());
			T* ptr = buffer + index;
			return *ptr;
		}

		bool IsValidIndex(const size_t index) const
		{
			size_t convertedIndex = index * Capacity;
			return convertedIndex >= 0 && convertedIndex < m_buffer.GetBufferSize();
		}

	private:
		Buffer m_buffer;
	};
}