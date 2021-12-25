#pragma once

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

		void Update(float deltaTime);

		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);

	private:
		std::vector<entt::entity> m_markedForDestroyEntities;
		entt::registry m_entityRegistry;

		friend class Entity;
	};
}