#pragma once

#include "EngineTime.h"
#include "Entity.h"

namespace Engine
{
	class BehaviorScriptContainer;

	class BehaviorScript
	{
	public:
		explicit BehaviorScript() = default;
		virtual ~BehaviorScript() { }

	public:
		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(bool enabled);

		const Entity& GetEntity() const;
		const BehaviorScriptContainer& GetBehaviors() const;

	protected:
		// On Create is called when object is created.
		virtual void OnCreate() { }
		virtual void OnEnable() { }
		virtual void OnDisable() { }
		virtual void OnDestroy() { }

		virtual void OnRuntimeUpdate(const Timestep& ts) { }
		virtual void OnEditorUpdate(const Timestep& ts) { }

		virtual BehaviorScript* CopyTo()=0;

	private:
		void SetOwningContainer(class BehaviorScriptContainer* container)
		{
			m_owningContainer = container;
		}

	private:
		BehaviorScriptContainer* m_owningContainer;
		bool m_enabled = true;
		bool m_triggeredOnCreate = false;

		friend class BehaviorScriptContainer;
	};
}