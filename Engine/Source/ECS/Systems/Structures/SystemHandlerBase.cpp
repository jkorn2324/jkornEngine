#include "EnginePCH.h"
#include "SystemHandlerBase.h"
#include "ISystemBase.h"

namespace Engine
{

	SystemHandlerBase::~SystemHandlerBase()
	{
		size_t systemSize = m_systems.size();
		for (size_t index = 0; index < systemSize; ++index)
		{
			ISystemBase* system = m_systems[index];
			// Deallocates the systems.
			delete system;
		}
	}
}