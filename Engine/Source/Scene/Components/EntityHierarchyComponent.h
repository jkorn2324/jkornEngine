#pragma once

#include "Entity.h"
#include "EventInvoker.h"

#include <vector>

namespace Engine
{
	class EntityHierarchyComponent;

	namespace EHC::Internal
	{
		void InvokeHierarchyChanged(Entity& entity, EntityHierarchyComponent& hierarchyChangedEvent);
	}

	template<typename TRegistry>
	class TEntityRef;

	// Used to store relationships between entities.
	class EntityHierarchyComponent
	{
	public:
		explicit EntityHierarchyComponent()
			: m_parentEntity(), m_children(), m_owner() { }
		explicit EntityHierarchyComponent(const Entity& owner)
			: m_parentEntity(), m_children(), m_owner(owner) { }

		EntityHierarchyComponent(const EntityHierarchyComponent& component)
			: m_parentEntity(component.m_parentEntity), m_children(component.m_children), m_owner(component.m_owner) { }

		EntityHierarchyComponent& operator=(const EntityHierarchyComponent& cpy)
		{
			m_parentEntity = cpy.m_parentEntity;
			m_children = cpy.m_children;
			m_owner = cpy.m_owner;
			return *this;
		}

		template<typename TRegistry = entt::registry>
		void SetParent(const Entity& entity, TRegistry& registry)
		{
			if (entity == m_parentEntity) { return; }

			if (m_parentEntity.IsValid<TRegistry>(registry))
			{
				EntityHierarchyComponent& component =
					m_parentEntity.GetComponent<EntityHierarchyComponent, TRegistry>(registry);
				const auto child = std::find(
					component.m_children.begin(), component.m_children.end(), m_owner);
				if (child != component.m_children.end())
				{
					component.m_children.erase(child);
				}
			}
			m_parentEntity = entity;

			if (m_parentEntity.IsValid<TRegistry>(registry))
			{
				EntityHierarchyComponent& component =
					m_parentEntity.GetComponent<EntityHierarchyComponent>(registry);
				component.m_children.push_back(m_owner);
			}
			EHC::Internal::InvokeHierarchyChanged(m_owner, *this);
		}

		template<typename TRegistry = entt::registry>
		void SetParent(TEntityRef<TRegistry>& entityRef)
		{
			SetParent<TRegistry>(entityRef.GetEntity(), entityRef.GetRegistry());
		}

		template<typename TRegistry = entt::registry>
		bool HasParent(TRegistry& registry) const
		{
			return m_parentEntity.IsValid(registry);
		}

		const Entity& GetParent() const { return m_parentEntity; }

		bool HasChildren() const { return m_children.size() > 0; }
		bool ContainsChild(const Entity& e);

		const std::vector<Entity>& GetChildren() const { return m_children; }

		const uint32_t GetNumChildren() const { return (uint32_t)m_children.size(); }

		const Entity& GetOwner() const { return m_owner; }

	private:
		std::vector<Entity> m_children;
		Entity m_parentEntity;
		Entity m_owner;
	};
}