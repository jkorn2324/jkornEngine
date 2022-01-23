#include "EnginePCH.h"
#include "Entity.h"

#include "Scene.h"

namespace Engine
{
    const Entity Entity::None = Entity(entt::null, nullptr);

    EventFunc Entity::s_componentEventFunc = nullptr;

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
    
    void Entity::BindEventFunc(const EventFunc& func)
    {
        s_componentEventFunc = func;
    }
}