#include "EnginePCH.h"
#include "Entity.h"
#include "Components\Component.h"

namespace Engine
{
    Entity::Entity()
        : m_components(),
        m_transform()
    {
    }

    Entity::~Entity()
    {
        std::int32_t size = m_components.size() - 1;
        while (size >= 0)
        {
            Component* component = m_components[size];
            m_components.pop_back();
            delete component;
            size--;
        }
    }

    void Entity::Update(const float& deltaTime)
    {
        // Updates the components in the entity.
        std::int32_t size = m_components.size() - 1;
        while (size >= 0)
        {
            Component* component = m_components[size];
            component->Update(deltaTime);
            size--;
        }
    }

    const MathLib::Transform3D& Entity::GetTransform() const
    {
        return m_transform;
    }

    void Entity::AddComponent(Component* component)
    {
        const auto& found = std::find(m_components.begin(), m_components.end(), component);
        if (found == m_components.end())
        {
            m_components.push_back(component);
        }
    }

    void Entity::RemoveComponent(Component* component)
    {
        const auto& found = std::find(m_components.begin(), 
            m_components.end(), component);
        if (found != m_components.end())
        {
            m_components.erase(found);
        }
    }
}