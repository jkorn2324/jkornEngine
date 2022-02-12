#pragma once

#include "Entity.h"

#include <memory>
#include <vector>

namespace Engine
{
	class BehaviorScript;

	class BehaviorScriptContainer
	{
	public:
		BehaviorScriptContainer(const Entity& entity);
		~BehaviorScriptContainer();

		template<typename T>
		T* GetBehavior() const
		{
			for (const auto& a : m_scripts)
			{
				T* dynamicCast = dynamic_cast<T*>(a);
				if (dynamicCast != nullptr)
				{
					return dynamicCast;
				}
			}
			return nullptr;
		}

		const std::vector<BehaviorScript*>& GetBehaviors() const { return m_scripts; }

		template<typename T>
		bool HasBehavior() const
		{
			T* behavior = GetBehavior();
			return behavior != nullptr;
		}

		template<typename T>
		T* AddBehavior()
		{
			T* behavior = GetBehavior<T>();
			if (behavior != nullptr) { return behavior; }

			behavior = new T();
			BehaviorScript* behaviorScript = dynamic_cast<BehaviorScript*>(behavior);
			if (behaviorScript != nullptr)
			{
				behaviorScript->SetOwningContainer(this);
				m_scripts.push_back(behaviorScript);
				return behavior;
			}
			return nullptr;
		}

		template<typename T, typename ...Args>
		T* AddBehavior(Args&&...args)
		{
			T* behavior = GetBehavior<T>();
			if (behavior != nullptr) { return behavior; }

			behavior = new T(std::forward<Args>(args)...);
			BehaviorScript* behaviorScript = dynamic_cast<BehaviorScript*>(behavior);
			if (behaviorScript != nullptr)
			{
				behaviorScript->SetOwningContainer(this);
				m_scripts.push_back(behaviorScript);
				return behavior;
			}
			return nullptr;
		}

		const Entity& GetEntity() const { return m_entity; }

	private:
		void CopyBehavior(BehaviorScript* behaviorScript);

		void OnUpdate(const Timestep& ts);
		void OnRuntimeUpdate(const Timestep& ts);
		void OnEditorUpdate(const Timestep& ts);
		void OnDestroy();

		void Deallocate();

	private:
		std::vector<class BehaviorScript*> m_scripts;
		Entity m_entity;
		bool m_runtime = false;

		friend class Scene;
		friend class BehaviorComponent;
	};
}