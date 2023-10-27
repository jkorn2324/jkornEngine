#include "EnginePCH.h"
#include "SystemManager.h"

namespace Engine
{
	SystemHandlerBase& SystemManager::Get()
	{
		static SystemHandlerBase s_systemManager;
		return s_systemManager;
	}
}