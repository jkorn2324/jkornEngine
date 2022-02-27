#include "EnginePCH.h"
#include "GUID.h"

#include <random>

namespace Engine
{

	static std::random_device s_randomDevice;
	static std::mt19937_64 s_generator(s_randomDevice());
	static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

	GUID::GUID()
		: m_guid(s_uniformDistribution(s_generator))
	{

	}
}