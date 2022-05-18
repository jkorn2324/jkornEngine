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
			: m_length(length * stride), m_buffer(nullptr)
		{
			m_buffer = new char[m_length];
		}
		
		FixedArray(const FixedArray& arr)
			: m_length(arr.m_length)
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

		template<typename T>
		bool Get(uint32_t index, T& out) const
		{
			// TODO: Fix bug where users are unable to get pixel data from this
			uint32_t convertedIndex = index * sizeof(T);
			if (convertedIndex >= GetLength()) return false;
			char* ptrPosition = m_buffer + convertedIndex;
			std::memcpy(&out, ptrPosition, sizeof(T));
			return true;
		}

		template<typename T>
		void Set(uint32_t index, const T& in)
		{
			uint32_t convertedIndex = index * sizeof(T);
			if (convertedIndex >= GetLength()) return;
			char* ptrPosition = m_buffer + convertedIndex;
			std::memcpy(ptrPosition, &in, sizeof(T));
		}

		uint32_t GetLength() const { return m_length; }

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