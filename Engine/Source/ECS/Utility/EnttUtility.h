#pragma once

#include <entt/entt.hpp>
#include <utility>

namespace Engine::Entt
{

	static inline auto GetEntityStorage(const entt::registry& registry)
	{
		return registry.storage<entt::entity>();
	}

	// Gets the total num entities.
	static inline size_t GetTotalEntities(const entt::registry& registry)
	{
		return GetEntityStorage(registry)->size();
	}

	template<typename TComponent>
	static inline bool HasComponent(const entt::entity entity, const entt::registry& registry)
	{
		return registry.any_of<TComponent>(entity);
	}

	template<typename TComponent>
	static inline TComponent& GetComponent(const entt::entity entity, entt::registry& registry)
	{
		return registry.get<TComponent>(entity);
	}

	template<typename TComponent>
	static inline const TComponent& GetComponent(const entt::entity entity, const entt::registry& registry)
	{
		return registry.get<TComponent>(entity);
	}

	template<typename TComponent, typename ...TArgs>
	static inline TComponent& AddComponent(const entt::entity entity, entt::registry& registry, TArgs&&... args)
	{
		return registry.emplace<TComponent, TArgs...>(entity, std::forward<TArgs>(args)...);
	}

	template<typename TComponent>
	static inline void RemoveComponent(const entt::entity entity, entt::registry& registry)
	{
		if (!HasComponent<TComponent>(entity, registry))
		{
			return;
		}
		registry.remove<TComponent>(entity);
	}

	template<typename TComponent, typename ...TArgs, typename TFunc>
	static inline void RemoveComponent(const entt::entity entity, entt::registry& registry, const TFunc& callback, TArgs&&...args)
	{
		if (!HasComponent<TComponent>(entity, registry))
		{
			return;
		}
		TComponent& component = GetComponent<TComponent>(entity, registry);
		callback(component, std::forward<TArgs>(args)...);
		registry.remove<TComponent>(entity);
	}

	static inline bool IsValid(const entt::entity entity, const entt::registry& registry)
	{
		return registry.valid(entity);
	}
}