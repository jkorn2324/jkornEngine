#include "EnginePCH.h"
#include "Entity.h"

#include "Scene.h"

namespace Engine
{
    Entity::Entity(const entt::entity& entity, Scene* scene)
        : m_scene(scene),
        m_entity(entity)
    {
    }

    bool Entity::IsValid() const
    {
        return m_scene != nullptr
            && m_scene->m_entityRegistry.valid(m_entity);
    }
}