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
		bool callOnCreate = !m_runtime;
		m_runtime = true;

		for (const auto& a : m_scripts)
		{
			if (callOnCreate)
			{
				a->OnCreate();
				a->m_triggeredOnCreate = true;
			}
			else
			{
				if (a->IsEnabled())
				{
					a->OnRuntimeUpdate(ts);
				}
			}
		}
	}

	void BehaviorScriptContainer::OnEditorUpdate(const Timestep& ts)
	{
		m_runtime = false;

		for (const auto& a : m_scripts)
		{
			if (a->IsEnabled())
			{
				a->OnEditorUpdate(ts);
			}
		}
	}

	void BehaviorScriptContainer::CopyBehavior(BehaviorScript* behaviorScript)
	{
		BehaviorScript* newBehaviorScript = behaviorScript->CopyTo();
		if (newBehaviorScript != nullptr)
		{
			const auto& found = std::find(m_scripts.begin(), m_scripts.end(), newBehaviorScript);
			if (found != m_scripts.end())
			{
				delete newBehaviorScript;
				return;
			}
			newBehaviorScript->SetOwningContainer(this);
			m_scripts.push_back(newBehaviorScript);
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