#include "EnginePCH.h"
#include "SystemInvoker.h"
#include "SystemHandlerBase.h"
#include "ISystemBase.h"

namespace Engine
{
	std::vector<ISystemBase*>& Engine::SystemInvoker::GetSystems()
	{
		return m_handlerBase.m_systems;
	}
}


