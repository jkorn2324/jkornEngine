#include "EnginePCH.h"
#include "EntityHierarchyComponent.h"

#include "EntityEvents.h"

namespace Engine
{

	EventFunc EntityHierarchyComponent::s_hierarchyEventFunc = nullptr;

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