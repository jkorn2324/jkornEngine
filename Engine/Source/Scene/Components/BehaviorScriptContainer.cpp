#include "EnginePCH.h"
#include "BehaviorScriptContainer.h"
#include "BehaviorScript.h"

namespace Engine
{

	BehaviorScriptContainer::BehaviorScriptContainer(const Entity& entity)
		: m_entity(entity), m_scripts()
	{
	}

	BehaviorScriptContainer::~BehaviorScriptContainer()
	{
	}

	void BehaviorScriptContainer::OnRuntimeUpdate(const Timestep& ts)
	{
		for (const auto& a : m_scripts)
		{
			if (a->IsEnabled())
			{
				a->OnRuntimeUpdate(ts);
			}
		}
	}

	void BehaviorScriptContainer::OnEditorUpdate(const Timestep& ts)
	{
		for (const auto& a : m_scripts)
		{
			if (a->IsEnabled())
			{
				a->OnEditorUpdate(ts);
			}
		}
	}

	void BehaviorScriptContainer::OnUpdate(const Timestep& ts)
	{
		for (const auto& a : m_scripts)
		{
			if (!a->m_triggeredOnCreate)
			{
				a->OnCreate();
				a->m_triggeredOnCreate = true;
			}
		}
	}

	void BehaviorScriptContainer::OnDestroy()
	{
		for (const auto& a : m_scripts)
		{
			a->OnDestroy();
		}
	}
	
	void BehaviorScriptContainer::Deallocate()
	{
		for (BehaviorScript* script : m_scripts)
		{
			delete script;
		}
		m_scripts.clear();
	}
}