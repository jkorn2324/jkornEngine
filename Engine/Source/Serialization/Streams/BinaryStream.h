#pragma once

#include "IStream.h"
#include <filesystem>

namespace Engine
{

	/// <summary>
	/// Binary Stream Writer.
	/// </summary>
	class BinaryStreamWriter : public IStreamWriter
	{
	public:
		BinaryStreamWriter();
		BinaryStreamWriter(size_t capacity);
		virtual ~BinaryStreamWriter();

		template<typename T>
		void Write(const T& bytes)
		{
			Write(reinterpret_cast<void*>(&bytes), sizeof(T));
		}
		void Write(void* bytes, size_t size) override;
		void Reset() { Reset(0); }
		void Reset(size_t capacity);

		void SetPosition(size_t position);
		void Release();

		size_t GetLength() const { return m_capacity; }
		size_t GetCapacity() const { return m_capacity; }
		size_t GetPosition() const;
		uint8_t* GetRaw() const { return m_ptr; }

	private:
		uint8_t* m_ptr;
		uint8_t* m_ptrPos;
		size_t m_capacity;
	};

	/// <summary>
	/// Binary Stream Reader.
	/// </summary>
	class BinaryStreamReader : public IStreamReader
	{
	public:
		BinaryStreamReader();
		BinaryStreamReader(void* buffer, size_t capacity);
		~BinaryStreamReader();

		void Reset();
		void Reset(void* buffer, size_t capacity);

		template<typename T>
		void Read(T& tOutput)
		{
			void* buf = reinterpret_cast<void*>(&tOutput);
			Read(&buf, sizeof(T));
		}

		uint8_t* GetRawBuffer() const { return m_buffer; }

		bool Read(void** inBuf, size_t size) override;
		bool IsEndOfStream() const override;

		size_t GetPosition() const;
		void SetPosition(size_t position);

		size_t GetLength() const { return m_capacity; }

	private:
		uint8_t* m_buffer;
		uint8_t* m_bufferPosition;
		size_t m_capacity;
	};
}