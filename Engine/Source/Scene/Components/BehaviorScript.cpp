#include "EnginePCH.h"
#include "BehaviorScript.h"

#include "BehaviorScriptContainer.h"

namespace Engine
{
	
	void BehaviorScript::SetEnabled(bool enabled)
	{
		if (m_enabled != enabled)
		{
			if (enabled)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
			m_enabled = enabled;
		}
	}

	const Entity& BehaviorScript::GetEntity() const
	{
		return m_owningContainer->GetEntity();
	}

	const BehaviorScriptContainer& BehaviorScript::GetBehaviors() const
	{
		return *m_owningContainer;
	}
}
