#pragma once

#include "Memory.h"
#include "Allocator.h"

namespace Engine
{

	/**
	 * Represents a fixed runtime buffer. 
	 */
	template<typename TAllocator = DefaultAllocator>
	class FixedRuntimeBuffer
	{
	private:
		using Byte = uint8_t;

	public:
		FixedRuntimeBuffer(const size_t bufLength)
			: m_bufferBytes(nullptr), m_bufferLength(bufLength), m_allocator()
		{
			Allocate(bufLength);
		}

		/**
		 * Copy Constructor. 
		 */
		FixedRuntimeBuffer(const FixedRuntimeBuffer& buf)
			: m_bufferBytes(nullptr), m_bufferLength(buf.m_bufferLength), m_allocator()
		{
			Allocate(m_bufferLength);
			Memory::Memcpy(m_bufferBytes, buf.GetBuffer(), buf.m_bufferLength);
		}

		~FixedRuntimeBuffer()
		{
			DeAllocate(m_bufferLength);
		}

		/**
		 * Copy assignment operator. 
		 */
		FixedRuntimeBuffer& operator=(const FixedRuntimeBuffer& buf)
		{
			if (m_bufferLength != buf.m_bufferLength)
			{
				const size_t prevLen = m_bufferLength;
				m_bufferLength = buf.m_bufferLength;

				DeAllocate(prevLen);
				Allocate(m_bufferLength);
			}
			Memory::Memcpy(m_bufferBytes, buf.GetBuffer(), m_bufferLength);
			return *this;
		}

		const void* GetBuffer() const { return (void*)m_bufferBytes; }

		size_t GetBufferSize() const { return m_bufferLength; }

		template<typename TValue>
		TValue* Get(const size_t offset) const
		{
			if (offset >= m_bufferLength)
			{
				return nullptr;
			}
			return reinterpret_cast<TValue*>(m_bufferBytes + offset);
		}

		template<typename TValue>
		void Set(const TValue& value, const size_t offset)
		{
			size_t stride = sizeof(TValue);
			Set(reinterpret_cast<const void*>(&value), stride, offset);
		}

		void Set(const void* ptr, const size_t stride, const size_t offset)
		{
			// Do nothing if the ptr is null.
			if (!ptr)
			{
				return;
			}
			if (m_bufferBytes)
			{
				JKORN_ENGINE_ASSERT((offset + stride) < m_bufferLength, "Buffer will result in a heap allocation corruption.");
				void* bufferPtr = m_bufferBytes + offset;
				Memory::Memcpy(bufferPtr, ptr, stride);
			}
		}

	private:
		void Allocate(const size_t length)
		{
			m_bufferBytes = m_allocator.Allocate<Byte>(length);
			Memory::Memset(m_bufferBytes, 0, length);
		}

		void DeAllocate(const size_t length)
		{
			m_allocator.DeAllocate<Byte>(m_bufferBytes, length);
		}

	public:
		/**
		 * Clears the memory for the buffer.
		 */
		void ClearBuffer()
		{
			if (m_bufferBytes)
			{
				Memory::Memset(m_bufferBytes, 0, m_bufferLength);
			}
		}

	private:
		Byte* m_bufferBytes;
		TAllocator m_allocator;
		size_t m_bufferLength;
	};

	/**
	 * Represents a fixed compile time buffer. 
	 * 
	 * Deep copies from other buffers.
	 */
	template<size_t BufferSize>
	class FixedBuffer
	{
	private:
		using Byte = uint8_t;

	public:
		FixedBuffer()
		{
			ClearBuffer();
		}

		FixedBuffer(const FixedBuffer& buffer)
		{
			Memory::Memcpy(m_bufferBytes, buffer.m_bufferBytes, BufferSize);
		}

		FixedBuffer& operator=(const FixedBuffer& buf)
		{
			Memory::Memcpy(m_bufferBytes, buf.m_bufferBytes, BufferSize);
			return *this;
		}

		const void* GetBuffer() const { return (void*)&m_bufferBytes[0]; }

		/**
		 * Gets the buffer Size. 
		 */
		constexpr size_t GetBufferSize() const { return BufferSize; }

		template<typename TValue>
		TValue* Get(const size_t offset) const
		{
			if (offset >= m_bufferLength)
			{
				return nullptr;
			}
			return reinterpret_cast<TValue*>(m_bufferBytes + offset);
		}

		template<typename TValue>
		void Set(const TValue& value, const size_t offset)
		{
			size_t stride = sizeof(TValue);
			Set(reinterpret_cast<void*>(&value), stride, offset);
		}

		void Set(const void* ptr, const size_t stride, const size_t offset)
		{
			// Do nothing if the ptr is null.
			if (!ptr)
			{
				return;
			}

			JKORN_ENGINE_ASSERT((offset + stride) < BufferSize, "Buffer will result in a heap allocation corruption.");
			void* bufferPtr = m_bufferBytes + offset;
			Memory::Memcpy(m_bufferBytes, ptr, stride);
		}

		/**
		 * Clears the memory for the buffer. 
		 */
		void ClearBuffer()
		{
			Memory::Memset((void*)&m_bufferBytes[0], 0, GetBufferSize());
		}

	private:
		Byte m_bufferBytes[BufferSize];
	};

	template<typename TBuffer>
	struct IsValidBuffer
	{
		static constexpr bool Value = false;
	};

	template<typename TAllocator>
	struct IsValidBuffer<FixedRuntimeBuffer<TAllocator>>
	{
		static constexpr bool Value = true;
	};

	template<size_t size>
	struct IsValidBuffer<FixedBuffer<size>>
	{
		static constexpr bool Value = true;
	};

	// The default fixed runtime buffer.
	using DFixedRuntimeBuffer = FixedRuntimeBuffer<DefaultAllocator>;
}