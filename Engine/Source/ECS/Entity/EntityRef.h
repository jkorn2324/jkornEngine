#pragma once

#include "DebugAssert.h"
#include "Entity.h"
#include "ComponentUtility.h"

namespace Engine
{
	/**
	 * The entity reference.
	 */
	template<typename TRegistry = entt::registry>
	class TEntityRef
	{
		// Asserts that there must be a static registry.
		static_assert(IsValidRegistry<TRegistry>::value, "The registry must be a valid registry.");

	public:
		// Default Constructor.
		TEntityRef()
			: m_entity(), m_registry(nullptr) { }

		TEntityRef(const uint64_t id, TRegistry& registry)
			: m_entity((entt::entity)id), m_registry(&registry) { }

		TEntityRef(const uint32_t id, TRegistry& registry)
			: m_entity((entt::entity)id), m_registry(&registry) { }

		TEntityRef(const Entity& entity, TRegistry& registry)
			: m_entity(entity), m_registry(&registry) { }

	public:
		template<typename TComponent, typename ... TArgs>
		TComponent& AddComponent(TArgs&&... args)
		{
			return m_entity.AddComponent<TComponent, TArgs...>(GetRegistry(), std::forward<TArgs>(args)...);
		}

		template<typename TComponent>
		void RemoveComponent()
		{
			m_entity.RemoveComponent<TComponent>(GetRegistry());
		}

		template<typename TComponent>
		bool HasComponent() const
		{
			return m_entity.HasComponent<TComponent>(GetRegistry());
		}

		template<typename TComponent>
		TComponent& GetComponent()
		{
			return m_entity.GetComponent<TComponent>(GetRegistry());
		}

		template<typename TComponent>
		TComponent& GetComponent() const
		{
			return m_entity.GetComponent<TComponent>(GetRegistry());
		}

		const Entity GetEntity() const { return m_entity; }

		TRegistry& GetRegistry()
		{
			DebugAssert(m_registry != nullptr, "Registry is not nullptr");
			return *m_registry;
		}

		TRegistry& GetRegistry() const
		{
			DebugAssert(m_registry != nullptr, "Registry is not nullptr");
			return *m_registry;
		}

		bool IsValid() const { return m_registry != nullptr && m_entity.IsValid<TRegistry>(GetRegistry()); }

		entt::entity GetID() const { return (entt::entity)m_entity; }

		operator bool() const
		{
			return IsValid();
		}

		operator Entity() const
		{
			return m_entity;
		}

		friend bool operator==(const TEntityRef<TRegistry>& a, const TEntityRef<TRegistry>& b)
		{
			return a.m_entity == b.m_entity;
		}

		friend bool operator!=(const TEntityRef<TRegistry>& a, const TEntityRef<TRegistry>& b)
		{
			return a.m_entity != b.m_entity;
		}

		friend bool operator==(const Entity& a, const TEntityRef<TRegistry>& b)
		{
			return a == b.m_entity;
		}

		friend bool operator!=(const Entity& a, const TEntityRef<TRegistry>& b)
		{
			return a != b.m_entity;
		}

		friend bool operator==(const TEntityRef<TRegistry>& a, const Entity& b)
		{
			return a.m_entity == b;
		}

		friend bool operator!=(const TEntityRef<TRegistry>& a, const Entity& b)
		{
			return a.m_entity != b;
		}

	public:
		// Copies the entities.
		static void CopyEntity(TEntityRef<TRegistry>& from, TEntityRef<TRegistry>& to)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Can't copy entity as the registry is const.");
			Entity::CopyEntity(from, from.GetRegistry(), to, to.GetRegistry());
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		static void CopyComponent(TEntityRef<TRegistry>& from, TEntityRef<TRegistry>& to)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Can't copy entity as the registry is const.");
			Entity::CopyComponent<TRegistry, TComponent>(from, from.GetRegistry(), to, to.GetRegistry());
		}

		template<typename... TComponents>
		static void CopyComponents(TEntityRef<TRegistry>& from, TEntityRef<TRegistry>& to)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Can't copy entity as the registry is const.");
			Entity::CopyComponents<TRegistry, TComponents...>(from, from.GetRegistry(), to, to.GetRegistry());
		}

		// Determines whether or not its a const reference.
		static constexpr bool IsConst = IsValidRegistry<TRegistry>::value && IsValidRegistry<TRegistry>::is_const;

	private:
		Entity m_entity;
		TRegistry* m_registry = nullptr;
	};

	// Defines the types for the entity registry.
	using ConstEntityRef = TEntityRef<const entt::registry>;
	using EntityRef = TEntityRef<entt::registry>;
}