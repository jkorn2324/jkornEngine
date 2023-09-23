#pragma once

#include <entt/entt.hpp>
#include <utility>

// ----------------------- Reflection Generation For Copying Entities --------------------------

template<typename Type>
struct meta_mixin : Type {
	using allocator_type = typename Type::allocator_type;
	using value_type = typename Type::value_type;

	explicit meta_mixin(const allocator_type& allocator);
};

template<typename Type, typename Entity>
struct entt::storage_type<Type, Entity> {
	using type = meta_mixin<entt::basic_storage<Type, Entity>>;
};

template<typename Type>
meta_mixin<Type>::meta_mixin(const allocator_type& allocator)
	: Type{ allocator } {
	using namespace entt::literals;

	entt::meta<value_type>()
		// cross registry, same type
		.template func<entt::overload<entt::storage_for_t<value_type, entt::entity>& (const entt::id_type)>(&entt::basic_registry<entt::entity>::storage<value_type>), entt::as_ref_t>("storage"_hs);
}

// --------------------------------------------------------------------------------------------

namespace Engine
{
	template<typename TRegistry>
	struct IsValidRegistry
	{
		static constexpr bool value = false;
		static constexpr bool is_const = std::is_const<TRegistry>::value;
	};

	template<>
	struct IsValidRegistry<entt::registry>
	{
		static constexpr bool value = true;
		static constexpr bool is_const = false;
	};

	template<>
	struct IsValidRegistry<const entt::registry>
	{
		static constexpr bool value = true;
		static constexpr bool is_const = true;
	};

	namespace Entt
	{
		using namespace entt::internal;

		template<typename TComponent>
		static inline void ValidateComponent()
		{
			// Used to add reflection types to the components
			auto metaRegistry = entt::meta<TComponent>();
		}

		template<typename TRegistry = entt::registry>
		static inline auto GetEntityStorage(TRegistry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value, "Registry must be a valid registry.");
			return registry.storage<entt::entity>();
		}

		// Gets the total num entities.
		template<typename TRegistry = entt::registry>
		static inline size_t GetTotalEntities(TRegistry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value, "Registry must be a valid registry.");
			return GetEntityStorage(registry)->size();
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		static inline bool HasComponent(const entt::entity entity, TRegistry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value, "Registry must be a valid registry.");
			ValidateComponent<TComponent>();
			return registry.any_of<TComponent>(entity);
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		static inline TComponent& GetComponent(const entt::entity entity, entt::registry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Registry must be a valid registry & it must be non constant.");
			ValidateComponent<TComponent>();
			return registry.get<TComponent>(entity);
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		static inline const TComponent& GetComponent(const entt::entity entity, const TRegistry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Registry must be a valid registry & it must be a non constant.");
			ValidateComponent<TComponent>();
			return registry.get<TComponent>(entity);
		}

		template<typename TComponent, typename ...TArgs, typename TRegistry = entt::registry>
		static inline TComponent& AddComponent(const entt::entity entity, TRegistry& registry, TArgs&&... args)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Registry must be a valid registry & it must be non constant.");
			ValidateComponent<TComponent>();
			return registry.emplace<TComponent, TArgs...>(entity, std::forward<TArgs>(args)...);
		}

		template<typename TComponent, typename TRegistry = entt::registry>
		static inline void RemoveComponent(const entt::entity entity, entt::registry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Registry must be a valid registry & it must be non constant.");
			ValidateComponent<TComponent>();
			if (!HasComponent<TComponent, TRegistry>(entity, registry))
			{
				return;
			}
			registry.remove<TComponent>(entity);
		}

		template<typename TComponent, typename ...TArgs, typename TFunc, typename TRegistry = entt::registry>
		static inline void RemoveComponent(const entt::entity entity, TRegistry& registry, const TFunc& callback, TArgs&&...args)
		{
			static_assert(IsValidRegistry<TRegistry>::value && !IsValidRegistry<TRegistry>::is_const,
				"Registry must be a valid registry & it must be non constant.");
			ValidateComponent<TComponent>();
			if (!HasComponent<TComponent>(entity, registry))
			{
				return;
			}
			TComponent& component = GetComponent<TComponent>(entity, registry);
			callback(component, std::forward<TArgs>(args)...);
			registry.remove<TComponent>(entity);
		}

		template<typename TComponent, typename TRegistry, typename TFunc>
		static inline void CopyComponent(const entt::entity from, TRegistry& fromRegistry, const entt::entity to, TRegistry& toRegistry,
			const TFunc& copyFunc, bool createIfNotExist = false)
		{
			ValidateComponent<TComponent>();
			if (!HasComponent<TComponent, TRegistry>(from, fromRegistry))
			{
				return;
			}
			TComponent& component = GetComponent<TComponent, TRegistry>(from, fromRegistry);
			if (!HasComponent<TComponent, TRegistry>(to, toRegistry))
			{
				if (!createIfNotExist)
				{
					return;
				}
				TComponent& copyComponent = AddComponent<TComponent, TRegistry>(to, toRegistry);
				copyFunc(component, copyComponent);
				return;
			}
			TComponent& copyComponent = GetComponent<TComponent, TRegistry>(to, toRegistry);
			copyFunc(component, copyComponent);
		}


		template<typename TRegistry = entt::registry>
		static inline bool IsValid(const entt::entity entity, TRegistry& registry)
		{
			static_assert(IsValidRegistry<TRegistry>::value, "Registry must be a valid registry.");
			return registry.valid(entity);
		}

		// Copies the entity from one registry to an entity from another registry.
		template<typename TRegistry = entt::registry>
		static inline void CopyEntity(const entt::entity from, TRegistry& fromRegistry, const entt::entity to, TRegistry& toRegistry)
		{
			static_assert(IsValidRegistry<TRegistry>::value, "Registry must be a valid registry.");
			if (!IsValid(from, fromRegistry) || !IsValid(to, toRegistry))
			{
				return;
			}
			for (auto [id, storage] : fromRegistry.storage())
			{
				if (storage.contains(from))
				{
					auto* other = toRegistry.storage(id);
					if (!other)
					{
						using namespace entt::literals;
						auto storageType = entt::resolve(storage.type());
						// Invokes on the meta type for the storage type.
						storageType.invoke("storage"_hs, {}, entt::forward_as_meta(toRegistry), id);
						other = toRegistry.storage(id);
					}

					// If contains the to, remove it.
					if (other->contains(to))
					{
						other->remove(to);
					}
					// Storage Ptr for the entity.
					other->push(to, storage.value(from));
				}
			}
		}
	}
}