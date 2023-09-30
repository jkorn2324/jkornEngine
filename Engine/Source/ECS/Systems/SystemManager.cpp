#include "EnginePCH.h"
#include "SystemManager.h"

#include "IUpdateSystem.h"
#include "SceneManager.h"

namespace Engine
{
	SystemHandlerBase& SystemManager::Get()
	{
		static SystemHandlerBase s_systemManager;
		return s_systemManager;
	}

	namespace SystemUtility
	{
		void InvokeOnUpdate(const Engine::Timestep& timestep, const bool isPlaying)
		{
			Engine::UpdateSystemContext updateSystemContext = { Engine::SceneManager::GetActiveScene(), timestep, isPlaying };
			InvokeOnUpdate(updateSystemContext);
		}

		void InvokeOnUpdate(const Engine::UpdateSystemContext& updateSystemContext)
		{
			Engine::SystemManager::Invoke<Engine::IUpdateSystemBase>(
				[](Engine::IUpdateSystemBase& updateSystem, const Engine::UpdateSystemContext& context) -> void {
					updateSystem.InvokeOnUpdate(context);
				}, updateSystemContext);
		}
	}
}