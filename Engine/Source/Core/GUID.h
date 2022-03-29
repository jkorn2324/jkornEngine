#pragma once

#include <stdint.h>
#include <xhash>

namespace Engine
{
	// 64 bit guid.
	class GUID
	{
	public:
		GUID();
		GUID(const GUID& guid)
			: m_guid(guid.m_guid) { }
		GUID(uint64_t guid)
			: m_guid(guid) { }

		operator uint64_t() { return m_guid; }
		operator const uint64_t() const { return m_guid; }
		
		friend bool operator==(const GUID& a, const GUID& b)
		{
			return (uint64_t)a == (uint64_t)b;
		}

		friend bool operator==(const GUID& a, const uint64_t& b)
		{
			return (uint64_t)a == b;
		}

		friend std::wstring ToWString(const GUID& guid);

	private:
		uint64_t m_guid;
	};
}

namespace std
{

	template<>
	struct hash<Engine::GUID>
	{
		std::size_t operator()(const Engine::GUID& guid) const
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}