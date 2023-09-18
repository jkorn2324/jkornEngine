#pragma once

#include "EnttUtility.h"
#include "ComponentUtility.h"
#include "EntityEvents.h"

namespace Engine
{
	/**
	 * The entity class.
	 */
	class Entity
	{
	public:
		Entity() : m_entity(entt::null) { }
		Entity(const uint32_t id)
			: m_entity((entt::entity)id) { }
		Entity(const entt::entity entity)
			: m_entity(entity) { }

		template<typename TComponent, typename TRegistry = entt::registry>
		bool HasComponent(TRegistry& registry) const
		{
			return Entt::HasComponent<TComponent>(m_entity, registry);
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		void RemoveComponent(TRegistry& registry)
		{
			Entt::RemoveComponent<TComponent>(m_entity, registry,
				[](TComponent& component, Entity& entity) -> void {
					if (s_componentEventFunc != nullptr)
					{
						EntityComponentRemovedEvent<TComponent> event(entity, component);
						s_componentEventFunc(event);
					}
				}, *this);
		}

		template<typename TComponent, typename ... TArgs, typename TRegistry = entt::registry>
		TComponent& AddComponent(TRegistry& registry, TArgs&&... args)
		{
			TComponent& component = Entt::AddComponent<TComponent, TArgs...>(m_entity, registry, std::forward<TArgs>(args)...);
			if (s_componentEventFunc != nullptr)
			{
				EntityComponentAddedEvent<TComponent> event(*this, component);
				s_componentEventFunc(event);
			}
			return component;
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		TComponent& GetComponent(TRegistry& registry)
		{
			return Entt::GetComponent<TComponent>(m_entity, registry);
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		TComponent& GetComponent(TRegistry& registry) const
		{
			return Entt::GetComponent<TComponent>(m_entity, registry);
		}

		template<typename TRegistry = entt::registry>
		bool IsValid(TRegistry& registry) const
		{
			return Entt::IsValid(m_entity, registry);
		}

		friend bool operator==(const Entity& a, const Entity& b)
		{
			return a.m_entity == b.m_entity;
		}

		friend bool operator!=(const Entity& a, const Entity& b)
		{
			return a.m_entity != b.m_entity;
		}

		explicit operator entt::entity() const
		{
			return m_entity;
		}
	private:
		entt::entity m_entity;

		/** ----------------------- STATICS --------------------- */

	public:
		// The none entity.
		static const Entity None;

	private:
		static EventFunc s_componentEventFunc;

	public:
		static void BindEventFunc(const EventFunc& eventFunc);

		template<typename TComponent, typename TRegistry = entt::registry>
		static void CopyComponent(Entity& from, TRegistry& fromRegistry, Entity& to, TRegistry& toRegistry)
		{
			static_assert(IsValidComponent<TComponent>::value, "TComponent must be a valid component.");

			if (!from.HasComponent<TComponent>(fromRegistry))
			{
				return;
			}
			TComponent& component = from.GetComponent<TComponent>(fromRegistry);
			if (!to.HasComponent<TComponent>(toRegistry))
			{
				TComponent& pastedComponent = to.AddComponent(toRegistry);
				// Assignable constructor.
				pastedComponent = component;
				return;
			}
			TComponent& pastedComponent = to.GetComponent<TComponent>(toRegistry);
			// Assignable constructor.
			pastedComponent = component;
		}

		template<typename TRegistry = entt::registry>
		static void CopyEntity(const Entity& from, TRegistry& fromRegistry, const Entity& to, TRegistry& toRegistry)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const, "Registry must be a valid non constant registry.");
			Entt::CopyEntity((entt::entity)from, fromRegistry, (entt::entity)to, toRegistry);
		}

		template<typename ... TComponents, typename TRegistry = entt::registry>
		static void CopyComponents(Entity& from, TRegistry& fromRegistry, Entity& to, TRegistry& toRegistry)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const, "Registry must be a valid non constant registry.");
			CopyComponent<TComponents, TRegistry>(from, fromRegistry, to, toRegistry)...;
		}
	};
}