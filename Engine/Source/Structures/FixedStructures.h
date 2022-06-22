#pragma once

#include <stdint.h>
#include <memory>

namespace Engine
{

	class FixedArray
	{
	public:
		FixedArray()
			: m_length(0), m_buffer(nullptr)
		{
		}

		FixedArray(uint32_t length, size_t stride)
			: m_length(length * (uint32_t)stride), m_buffer(nullptr)
		{
			m_buffer = new char[m_length];
		}
		
		FixedArray(const FixedArray& arr)
			: m_length(arr.m_length), m_buffer(nullptr)
		{
			if (m_length > 0)
			{
				m_buffer = new char[m_length];
				std::memcpy(m_buffer, arr.m_buffer, m_length);
			}
		}

		~FixedArray() { Free(); }

		FixedArray& operator=(const FixedArray& fixedArray)
		{
			if (fixedArray.m_buffer == nullptr) return *this;
			if (fixedArray.GetLength() != m_length)
			{
				Free();
				m_buffer = new char[fixedArray.m_length];
				m_length = fixedArray.m_length;
			}
			std::memcpy(m_buffer, fixedArray.m_buffer, fixedArray.m_length);
			return *this;
		}

		void CopyBuffer(const void* buffer)
		{
			if (m_buffer == nullptr) return;
			std::memcpy(m_buffer, buffer, m_length);
		}

		template<typename T>
		bool Get(uint32_t index, T& out) const
		{
			uint32_t convertedLength = GetLength() / (uint32_t)sizeof(T);
			if (index >= convertedLength) return false;
			T* casted = reinterpret_cast<T*>(m_buffer) + index;
			out = casted[0];
			return true;
		}

		template<typename T>
		void Set(uint32_t index, const T& in)
		{
			uint32_t convertedIndex = index * (uint32_t)sizeof(T);
			if (convertedIndex >= GetLength()) return;
			T* ptrPosition = reinterpret_cast<T*>(m_buffer + convertedIndex);
			std::memcpy(ptrPosition, &in, sizeof(T));
		}

		uint32_t GetLength() const { return m_length; }

		uint8_t* GetRawBuffer() const { return reinterpret_cast<uint8_t*>(m_buffer); }

	private:
		void Free()
		{
			if (m_buffer != nullptr)
			{
				delete[] m_buffer;
			}
			m_length = 0;
		}

	protected:
		char* m_buffer;
		uint32_t m_length;
	};
}