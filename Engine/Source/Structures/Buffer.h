#pragma once

#include "EngineAssert.h"
#include "Memory.h"
#include "Allocator.h"

namespace Engine
{
	/**
	 * The buffer modifier class (modifies the buffer). 
	 */
	class BufferModifier
	{
	public:
		using Byte = uint8_t;

	public:
		BufferModifier(Byte*& buffer, size_t length)
			: m_bufferBytes(buffer), m_bufferLength(length)
		{

		}

		void* GetBuffer() { return m_bufferBytes; }

		template<typename TValue>
		void SetViaIndex(const TValue& value, const size_t index)
		{
			size_t offset = index * sizeof(TValue);
			Set(value, offset);
		}

		template<typename TValue>
		void Set(const TValue& value, const size_t offset)
		{
			size_t stride = sizeof(TValue);
			Set(&value, stride, offset);
		}

		void Set(const void* ptr, const size_t size, const size_t offset)
		{
			// Do nothing if the ptr is null.
			if (!ptr)
			{
				return;
			}
			if (m_bufferBytes)
			{
				JKORN_ENGINE_ASSERT((offset + size) <= m_bufferLength, "Buffer will result in a heap allocation corruption.");
				Byte* bufferOffset = m_bufferBytes + offset;
				Memory::Memcpy(bufferOffset, ptr, size);
			}
		}

	public:
		Byte*& m_bufferBytes;
		size_t m_bufferLength;

	public:

		/**
		 * Creates a buffer view from a buffer.
		 */
		template<typename TBuffer>
		static BufferModifier Create(TBuffer& buffer)
		{
			static_assert(IsValidBuffer<TBuffer>::Value, "The buffer must be a valid buffer.");
			return TBuffer::CreateModifier(buffer);
		}
	};

	/**
	 * The a view for a buffer (only views the buffer).
	 */
	class BufferView
	{
	public:
		using Byte = uint8_t;

		BufferView(void* buffer, size_t bufferLength)
			: m_bufferBytes(buffer), m_bufferLength(bufferLength)
		{

		}

		template<typename TValue>
		TValue* GetViaIndex(const size_t index) const
		{
			size_t offset = sizeof(TValue) * index;
			return Get<TValue>(offset);
		}

		template<typename TValue>
		bool GetViaIndex(const size_t index, TValue& value) const
		{
			TValue* ptr = GetViaIndex<TValue>(index);
			if (ptr)
			{
				value = *ptr;
				return true;
			}
			return false;
		}

		template<typename TValue>
		TValue* Get(const size_t offset) const
		{
			if (offset >= m_bufferLength)
			{
				return nullptr;
			}
			Byte* bufBytes = (Byte*)m_bufferBytes;
			return reinterpret_cast<TValue*>(bufBytes + offset);
		}

		template<typename TValue>
		bool Get(const size_t offset, TValue& value) const
		{
			TValue* ptr = Get<TValue>(offset);
			if (ptr)
			{
				value = *ptr;
				return true;
			}
			return false;
		}

		const void* GetBuffer() const { return m_bufferBytes; }

	private:
		void* m_bufferBytes;
		const size_t& m_bufferLength;

	public:

		/**
		 * Creates a buffer view from a buffer.
		 */
		template<typename TBuffer>
		static BufferView Create(TBuffer& buffer)
		{
			static_assert(IsValidBuffer<TBuffer>::Value, "The buffer must be a valid buffer.");
			return BufferView(buffer.GetBuffer(), buffer.GetBufferSize());
		}
	};


    template<typename TAllocator = DefaultAllocator>
    class ResizableBuffer
    {
        static_assert(IsAllocator<TAllocator>::Value, "The type of allocator must be a valid allocator.");
        
    public:
        using Byte = uint8_t;
        static const size_t DefaultCapacity = 32;
        static constexpr size_t DefaultReSizeAmount = 16;
        
    public:
        ResizableBuffer() : ResizableBuffer(DefaultCapacity) { }
        
        ResizableBuffer(const size_t capacity) : m_bufferBytes(nullptr), m_capacity(capacity), m_bufferSize(capacity)
        {
            Allocate(m_bufferBytes, capacity);
        }
        
        ResizableBuffer(const size_t capacity, TAllocator& allocator) 
            : m_bufferBytes(nullptr), m_allocator(allocator), m_capacity(capacity), m_bufferSize(capacity)
        {
            Allocate(m_bufferBytes, capacity);
        }
        
        ~ResizableBuffer()
        {
            DeAllocate(m_bufferBytes, m_capacity);
        }
        
        ResizableBuffer(const ResizableBuffer& buf)
            : m_bufferBytes(nullptr), m_allocator(buf.m_allocator), m_capacity(buf.m_capacity), m_bufferSize(buf.m_bufferSize)
        {
            Allocate(m_bufferBytes, m_capacity);
            Memory::Memcpy(m_bufferBytes, buf.m_bufferBytes, m_capacity);
        }
        
        ResizableBuffer& operator=(const ResizableBuffer& buf)
        {
            m_allocator = buf.m_allocator;
            if (m_capacity < buf.m_capacity)
            {
                Byte* bytes;
                Allocate(bytes, buf.m_capacity);
                DeAllocate(m_bufferBytes, m_capacity);
                m_bufferBytes = bytes;
            }
            m_capacity = buf.m_capacity;
            m_bufferSize = buf.m_bufferSize;
            Memory::Memcpy(m_bufferBytes, buf.m_bufferBytes, m_capacity);
            return *this;
        }
        
        template<size_t ReSizeAmount=DefaultReSizeAmount>
        void Resize()
        {
            ReSize(ReSizeAmount);
        }
        
        void ReSize(const size_t resizeAmount)
        {
            // Sets the buffer size to the
            if (ReSizeAmount < m_bufferSize)
            {
                m_bufferSize = resizeAmount;
                return;
            }
            Byte* newBytes;
            Allocate(newBytes, resizeAmount);
            Memory::Memcpy(newBytes, m_bufferBytes, reSizeAmount);
            DeAllocate(m_bufferBytes, m_capacity);
            m_bufferBytes = newBytes;
            m_capacity = m_bufferSize = resizeAmount;
        }
        
        size_t GetCapacity() const { return m_capacity; }
        
        size_t GetBufferSize() const { return m_bufferSize; }
        
        bool IsValidIndex(size_t bufferIndex) const
        {
            return bufferIndex >= 0 && bufferIndex < m_bufferSize;
        }
        
        void* GetBuffer() const { return reinterpret_cast<void*>(m_bufferBytes); }
        
        void* GetBuffer() { return reinterpret_cast<void*>(m_bufferBytes); }
        
        template<typename TValue>
        TValue* GetViaIndex(const size_t index) const
        {
            BufferView view(GetBuffer(), m_bufferSize);
            return view.GetViaIndex<TValue>(index);
        }

        template<typename TValue>
        bool GetViaIndex(const size_t index, TValue& value) const
        {
            BufferView view(GetBuffer(), m_bufferSize);
            return view.GetViaIndex<TValue>(index, value);
        }

        template<typename TValue>
        void SetViaIndex(const TValue& value, const size_t index)
        {
            BufferModifier view = CreateModifier(this);
            view.SetViaIndex<TValue>(value, index);
        }

        template<typename TValue>
        TValue* Get(const size_t offset) const
        {
            BufferView view(GetBuffer(), m_bufferSize);
            return view.Get<TValue>(offset);
        }

        template<typename TValue>
        bool Get(const size_t offset, TValue& value) const
        {
            BufferView view(GetBuffer(), m_bufferSize);
            return view.Get<TValue>(offset, value);
        }

        template<typename TValue>
        void Set(const TValue& value, const size_t offset)
        {
            BufferModifier view = CreateModifier(*this);
            view.Set<TValue>(value, offset);
        }

        void Set(const void* ptr, const size_t stride, const size_t offset)
        {
            BufferModifier view = CreateModifier(*this);
            view.Set(ptr, stride, offset);
        }

        
    private:
        void Allocate(Byte*& bytes, const size_t capacity)
        {
            bytes = m_allocator.typename Allocate<Byte>(capacity);
            Memory::Memset(bytes, 0, capacity);
        }
        
        void DeAllocate(Byte*& bytes, const size_t capacity)
        {
            m_allocator.template DeAllocate<Byte>(bytes, capacity);
        }
        
    private:
        static BufferModifier CreateModifier(ResizableBuffer<TAllocator>& runtimeBuffer)
        {
            return BufferModifier(runtimeBuffer.m_bufferBytes, runtimeBuffer.m_bufferSize);
        }
            
    private:
        Byte* m_bufferBytes;
        TAllocator m_allocator;
        size_t m_capacity;
        size_t m_bufferSize;
    };

	/**
	 * Represents a fixed runtime buffer. 
	 */
	template<typename TAllocator = DefaultAllocator>
	class FixedRuntimeBuffer
	{
        static_assert(IsAllocator<TAllocator>::Value, "The type of allocator must be a valid allocator.");
        
	public:
		using Byte = uint8_t;

	public:
		FixedRuntimeBuffer()
			: m_bufferBytes(nullptr), m_bufferLength(0), m_allocator()
		{

		}

		FixedRuntimeBuffer(const size_t bufLength)
			: m_bufferBytes(nullptr), m_bufferLength(bufLength), m_allocator()
		{
			Allocate(bufLength);
		}

		FixedRuntimeBuffer(const size_t bufLength, TAllocator& allocator)
			: m_bufferBytes(nullptr), m_bufferLength(bufLength), m_allocator(allocator)
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
            m_bufferLength = buf.m_bufferLength;
			return *this;
		}

		void* GetBuffer() const { return reinterpret_cast<void*>(m_bufferBytes); }

		void* GetBuffer() { return reinterpret_cast<void*>(m_bufferBytes); }

		size_t GetBufferSize() const { return m_bufferLength; }

		template<typename TValue>
		TValue* GetViaIndex(const size_t index) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.GetViaIndex<TValue>(index);
		}

		template<typename TValue>
		bool GetViaIndex(const size_t index, TValue& value) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.GetViaIndex<TValue>(index, value);
		}

		template<typename TValue>
		void SetViaIndex(const TValue& value, const size_t index)
		{
			BufferModifier view = CreateModifier(this);
			view.SetViaIndex<TValue>(value, index);
		}

		template<typename TValue>
		TValue* Get(const size_t offset) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.Get<TValue>(offset);
		}

		template<typename TValue>
		bool Get(const size_t offset, TValue& value) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.Get<TValue>(offset, value);
		}

		template<typename TValue>
		void Set(const TValue& value, const size_t offset)
		{
			BufferModifier view = CreateModifier(*this);
			view.Set<TValue>(value, offset);
		}

		void Set(const void* ptr, const size_t stride, const size_t offset)
		{
			BufferModifier view = CreateModifier(*this);
			view.Set(ptr, stride, offset);
		}


	private:
		void Allocate(const size_t length)
		{
            m_bufferBytes = m_allocator.template Allocate<Byte>(length);
			Memory::Memset(m_bufferBytes, 0, length);
		}

		void DeAllocate(const size_t length)
		{
			m_allocator.template DeAllocate<Byte>(m_bufferBytes, length);
		}

	private:
		static BufferModifier CreateModifier(FixedRuntimeBuffer<TAllocator>& runtimeBuffer)
		{
			return BufferModifier(runtimeBuffer.m_bufferBytes, runtimeBuffer.m_bufferLength);
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

		friend class BufferModifier;
	};

	/**
	 * Represents a fixed compile time buffer. 
	 * 
	 * Deep copies from other buffers.
	 */
	template<size_t BufferSize>
	class FixedBuffer
	{
	public:
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

		void* GetBuffer() { return (void*)&m_bufferBytes[0]; }

		/**
		 * Gets the buffer Size. 
		 */
		constexpr size_t GetBufferSize() const { return BufferSize; }

		template<typename TValue>
		TValue* GetViaIndex(const size_t index) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.GetViaIndex<TValue>(index);
		}

		template<typename TValue>
		bool GetViaIndex(const size_t index, TValue& value) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.GetViaIndex<TValue>(index, value);
		}

		template<typename TValue>
		void SetViaIndex(const TValue& value, const size_t index)
		{
			BufferModifier view = CreateModifier(this);
			view.SetViaIndex<TValue>(value, index);
		}

		template<typename TValue>
		TValue* Get(const size_t offset) const
		{
			BufferView view(GetBuffer(), BufferSize);
			return view.Get<TValue>(offset);
		}

		template<typename TValue>
		bool Get(const size_t offset, TValue& value) const
		{
			BufferView view(GetBuffer(), m_bufferLength);
			return view.Get<TValue>(offset, value);
		}

		template<typename TValue>
		void Set(const TValue& value, const size_t offset)
		{
			BufferModifier view = CreateModifier(*this);
			view.Set<TValue>(value, offset);
		}

		void Set(const void* ptr, const size_t stride, const size_t offset)
		{
			BufferModifier view = CreateModifier(*this);
			view.Set(ptr, stride, offset);
		}

		/**
		 * Clears the memory for the buffer. 
		 */
		void ClearBuffer()
		{
			Memory::Memset((void*)&m_bufferBytes[0], 0, GetBufferSize());
		}

	private:
		static BufferModifier CreateModifier(FixedBuffer<BufferSize>& runtimeBuffer)
		{
			void** bufferBytes = reinterpret_cast<void**>(&runtimeBuffer.m_bufferBytes);
			return BufferModifier(bufferBytes, BufferSize);
		}

	private:
		Byte m_bufferBytes[BufferSize];

		friend class BufferModifier;
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

	template<typename TAllocator>
    struct IsValidBuffer<ResizableBuffer<TAllocator>>
    {
        static constexpr bool Value = true;
    };

}
