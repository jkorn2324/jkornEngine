#include "EnginePCH.h"
#include "SystemUtility.h"

#include "IUpdateSystem.h"
#include "SceneManager.h"

namespace Engine
{
    void SystemUtility::InvokeOnUpdate(const Engine::Timestep& timestep, const bool isPlaying)
    {
        Engine::UpdateSystemContext updateSystemContext = { Engine::SceneManager::GetActiveScene(), timestep, isPlaying };
        InvokeOnUpdate(updateSystemContext);
    }
    
    void SystemUtility::InvokeOnUpdate(const Engine::UpdateSystemContext& updateSystemContext)
    {
        Engine::SystemManager::Invoke<Engine::IUpdateSystemBase>(
            [](Engine::IUpdateSystemBase& updateSystem, const Engine::UpdateSystemContext& context) -> void {
                updateSystem.InvokeOnUpdate(context);
            }, updateSystemContext);
    }
}