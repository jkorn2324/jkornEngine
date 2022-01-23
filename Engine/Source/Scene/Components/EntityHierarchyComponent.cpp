#include "EnginePCH.h"
#include "EntityHierarchyComponent.h"

#include "SceneEvent.h"

namespace Engine
{
	
	static EventFunc s_hierarchyEventFunc = nullptr;

	void EntityHierarchyComponent::SetParent(const Entity& entity)
	{
		if (entity == m_parentEntity) { return; }

		if (m_parentEntity.IsValid())
		{
			EntityHierarchyComponent& component =
				m_parentEntity.GetComponent<EntityHierarchyComponent>();
			const auto child = std::find(
				component.m_children.begin(), component.m_children.end(), m_owner);
			if (child != component.m_children.end())
			{
				component.m_children.erase(child);
			}
		}
		m_parentEntity = entity;

		if (m_parentEntity.IsValid())
		{
			EntityHierarchyComponent& component =
				m_parentEntity.GetComponent<EntityHierarchyComponent>();
			component.m_children.push_back(m_owner);
		}
		if (s_hierarchyEventFunc)
		{
			EntityHierarchyChangedEvent event(*this);
			s_hierarchyEventFunc(event);
		}
	}

	bool EntityHierarchyComponent::ContainsChild(const Entity& e)
	{
		const auto& f = std::find(m_children.begin(), m_children.end(), e);
		return f != m_children.end();
	}
	
	void EntityHierarchyComponent::BindEventFunc(const EventFunc& eventFunc)
	{
		s_hierarchyEventFunc = eventFunc;
	}
}