#pragma once

#include "ISystemBase.h"
#include "SystemTypes.h"
#include "EnttUtility.h"

namespace Engine
{
	/**
	 * The time step.
	 */
	class Timestep;
	class Scene;
	class Entity;

	// The context for the update system.
	struct UpdateSystemContext
	{
		Scene& scene;
		const Timestep& timestep;

		UpdateSystemContext(Scene& scene, const Timestep& ts)
			: timestep(ts), scene(scene) { }
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
	public:
		using Components = std::tuple<TComponents&...>;

		void InvokeOnUpdate(const UpdateSystemContext& updateSystemContext)
		{
			Scene& scene = updateSystemContext.scene;
			entt::registry& registry = scene.m_entityRegistry;
			auto entityView = registry.view<TComponents...>();
			// Iterate through each entity view.
			for (auto e : entityView)
			{
				Entity entity(e, &scene);
				auto c = registry.get<TComponents...>(e);
				OnUpdate(updateSystemContext, entity, c);
			}
		}

	protected:
		// Called to invoke on Update.
		virtual void OnUpdate(const UpdateSystemContext& ctx, Entity& e, Components& components) = 0;
	};
}