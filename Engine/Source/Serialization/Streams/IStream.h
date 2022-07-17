#pragma once

#include <stdint.h>

namespace std::filesystem
{
	class path;
}

namespace Engine
{

	/// <summary>
	/// Interface for Stream Writers.
	/// </summary>
	class IStreamWriter
	{
	public:
		virtual ~IStreamWriter() { }

		virtual void Write(void* bytes, uint32_t size)=0;
	};

	/// <summary>
	/// Interface for Stream Readers.
	/// </summary>
	class IStreamReader
	{
	public:
		virtual bool Read(void** bytes, uint32_t size) =0;
		virtual bool IsEndOfStream() const =0;
	};
}