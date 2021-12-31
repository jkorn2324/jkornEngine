#pragma once

#include "Timestep.h"

#include <entt.hpp>
#include <vector>

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		void Update(const Timestep& ts);

		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);

		class Camera* GetCamera() const;

	private:
		std::vector<entt::entity> m_markedForDestroyEntities;
		entt::registry m_entityRegistry;

		class Camera* m_camera;

		friend class Entity;
	};
}