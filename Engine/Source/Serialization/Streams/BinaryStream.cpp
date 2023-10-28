#include "EnginePCH.h"
#include "BinaryStream.h"
#include "Memory.h"

#if defined(PLATFORM_WINDOWS)
#include <corecrt_wstdio.h>
#endif

namespace Engine
{
	static const size_t c_reallocMultiplier = 2;
	static const size_t c_defaultAlloc = 256;

	// ------------------------------ Binary Stream Writer ----------------------------------

	BinaryStreamWriter::BinaryStreamWriter()
		: m_capacity(0), m_ptr(nullptr), m_ptrPos(nullptr)
	{

	}

	BinaryStreamWriter::BinaryStreamWriter(size_t capacity)
		: m_capacity(capacity), m_ptr(nullptr), m_ptrPos(nullptr)
	{
		if (m_capacity > 0)
		{
			m_ptr = new uint8_t[m_capacity];
		}
		else
		{
			m_ptr = nullptr;
		}
		m_ptrPos = m_ptrPos;
	}

	BinaryStreamWriter::~BinaryStreamWriter()
	{
		Release();
	}

	void BinaryStreamWriter::Write(void* bytes, size_t size)
	{
		size_t position = GetPosition();
		size_t diffToLength = m_capacity - position;
		// Reallocates memory.
		if (diffToLength < size)
		{
			size_t allocSize = m_capacity != 0 ? m_capacity * c_reallocMultiplier : c_defaultAlloc;
			uint8_t* newRealloc = new uint8_t[allocSize];
			if (m_ptr != nullptr)
			{
				Memory::Memcpy(newRealloc, m_ptr, m_capacity);
				delete[] m_ptr;
			}
			m_ptr = newRealloc;
			m_ptrPos = newRealloc + position;
		}
		Memory::Memcpy(m_ptrPos, bytes, size);
		m_ptrPos += size;
	}

	void BinaryStreamWriter::Reset(size_t capacity)
	{
		if (capacity > m_capacity)
		{
			delete[] m_ptr;
			m_ptr = new uint8_t[capacity];
		}
		m_capacity = capacity;
		m_ptrPos = m_ptr;
	}
	
	void BinaryStreamWriter::SetPosition(size_t position)
	{
		m_ptrPos = m_ptrPos + position;
	}

	size_t BinaryStreamWriter::GetPosition() const
	{
		return m_ptr == nullptr ? 0 : m_ptrPos - m_ptr;
	}

	void BinaryStreamWriter::Release()
	{
		delete[] m_ptr;
		m_ptr = nullptr;
		m_ptrPos = nullptr;
		m_capacity = 0;
	}

	// ------------------------------ Binary Stream Reader ----------------------------------

	BinaryStreamReader::BinaryStreamReader()
		: m_buffer(nullptr), m_bufferPosition(nullptr), m_capacity(0)
	{
	}

	BinaryStreamReader::BinaryStreamReader(void* buffer, size_t capacity)
		: m_buffer(nullptr), m_capacity(capacity)
	{
		m_buffer = m_bufferPosition = reinterpret_cast<uint8_t*>(buffer);
		m_capacity = capacity;
	}

	BinaryStreamReader::~BinaryStreamReader()
	{
		m_buffer = nullptr;
		m_bufferPosition = nullptr;
		m_capacity = 0;
	}

	void BinaryStreamReader::Reset()
	{
		m_buffer = m_bufferPosition = nullptr;
		m_capacity = 0;
	}

	void BinaryStreamReader::Reset(void* buffer, size_t capacity)
	{
		m_buffer = m_bufferPosition = reinterpret_cast<uint8_t*>(buffer);
		m_capacity = capacity;
	}

	bool BinaryStreamReader::Read(void** inBuf, size_t size)
	{
		size_t currentPosition = GetPosition();
		size_t bytesLeft = m_capacity - currentPosition;
		if (bytesLeft < size) return false;
		Memory::Memcpy(*inBuf, m_bufferPosition, size);
		m_bufferPosition += size;
		return true;
	}

	bool BinaryStreamReader::IsEndOfStream() const { return GetPosition() >= GetLength(); }

	void BinaryStreamReader::SetPosition(size_t position)
	{
		size_t currentPosition = position >= m_capacity ? m_capacity : position;
		m_bufferPosition = m_buffer + currentPosition;
	}

	size_t BinaryStreamReader::GetPosition() const
	{
		if (m_buffer == nullptr) return 0;
		size_t difference = m_bufferPosition - m_buffer;
		return difference >= m_capacity ? m_capacity : difference;
	}
}