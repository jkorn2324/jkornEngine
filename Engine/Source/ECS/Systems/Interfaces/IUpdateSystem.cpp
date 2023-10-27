//
//  IUpdateSystem.cpp
//  Engine
//
//  Created by Justin Korn on 10/3/23.
//

#include "EnginePCH.h"
#include "IUpdateSystem.h"

#include "Scene.h"

namespace Engine
{
    namespace UpdateSystem::Internals
    {
        entt::registry& GetEntityRegistry(Scene& scene)
        {
            return Engine::SceneUtility::Internals::GetEntityRegistry(scene);
        }
    }
}
