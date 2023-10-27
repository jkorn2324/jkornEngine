#include "EnginePCH.h"
#include "EntityHierarchyComponent.h"

#include "EntityEvents.h"
#include "EventInvoker.h"

namespace Engine
{
	void EHC::Internal::InvokeHierarchyChanged(Entity& entity, EntityHierarchyComponent& hierarchyChangedEvent)
	{
		EntityHierarchyChangedEvent changedEvent(hierarchyChangedEvent);
		EventInvoker::Global().Invoke(changedEvent);
	}

	bool EntityHierarchyComponent::ContainsChild(const Entity& e)
	{
		const auto& f = std::find(m_children.begin(), m_children.end(), e);
		return f != m_children.end();
	}
}
