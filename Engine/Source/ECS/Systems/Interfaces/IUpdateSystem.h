#pragma once

#include "ISystemBase.h"
#include "SystemTypes.h"
#include "EntityRef.h"

namespace std
{
	// Forward declare the vector.
	template<typename TType, typename TAllocator>
	class vector;
}

namespace Engine
{
	/**
	 * The time step.
	 */
	class Timestep;
	class Scene;

	// The context for the update system.
	struct UpdateSystemContext
	{
		Scene& scene;
		const Timestep& timestep;
		// Determines whether or not its updating while playing.
		bool isPlaying : 1;

		UpdateSystemContext(Scene& scene, const Timestep& ts, bool inIsPlaying)
			: timestep(ts), scene(scene), isPlaying(inIsPlaying) { }
	};

	/**
	 * System that gets invoked during the update.
	 */
	class IUpdateSystemBase : public ISystemBase
	{
	public:
		// Invokes On Update.
		virtual void InvokeOnUpdate(const UpdateSystemContext& updateSystemContext) = 0;

	public:
		// Ensures that this is a valid system type.
		static constexpr ValidSystemType GetSystemType() { return Type_UpdateSystem; }
	};

	/**
	 * System that gets invoked during the update.
	 */
	template<typename...TComponents>
	class IUpdateSystem : public IUpdateSystemBase
	{
	private:
		template<typename... T>
		struct SystemType
		{
			using type = std::tuple<T&...>;
		};

		template<typename T>
		struct SystemType<T>
		{
			using type = T;
		};

	public:
		/**
		 * The component type information. 
		 */
		using Components = typename SystemType<TComponents...>::type;

	protected:
		/**
		 * Gets the component from the components type.
		 */
		template<size_t Index = 0>
		decltype(auto) GetComponent(Components& component)
		{
			if constexpr (std::is_same<std::tuple<TComponents&...>, Components>::value)
			{
				return std::get<Index>(component);
			}
			return component;
		}

	public:
		void InvokeOnUpdate(const UpdateSystemContext& updateSystemContext)
		{
			Scene& scene = updateSystemContext.scene;
			entt::registry& registry = scene.m_entityRegistry;
			auto entityView = registry.view<TComponents...>();
			// Iterate through each entity view.
			for (auto e : entityView)
			{
				EntityRef entity(e, registry);
                auto c = entityView.template get<TComponents...>(e);
				OnUpdate(updateSystemContext, entity, c);
			}
		}

	protected:
		// Called to invoke on Update.
		virtual void OnUpdate(const UpdateSystemContext& ctx, EntityRef& e, Components& components) = 0;
	};
}
