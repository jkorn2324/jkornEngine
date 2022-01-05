#pragma once

#include <vector>
#include <memory>
#include <utility>

#include <entt.hpp>

#include "Scene.h"

namespace Engine
{

	class Entity
	{
	public:
		explicit Entity() = default;
		explicit Entity(const entt::entity& entity, class Scene* scene);

		template<typename T>
		T& GetComponent() const;
		template<typename T>
		T& GetComponent();
		template<typename T>
		bool HasComponent() const;
		template<typename T, typename... Args> 
		T& AddComponent(Args&&... args);
		template<typename T>
		void RemoveComponent();

		bool IsValid() const;

	private:
		class Scene* m_scene;
		entt::entity m_entity{ entt::null };

		friend class Scene;
	};

	template<typename T>
	T& Entity::GetComponent() const
	{
		return m_scene->m_entityRegistry.get<T>(m_entity);
	}

	template<typename T>
	T& Entity::GetComponent()
	{
		return m_scene->m_entityRegistry.get<T>(m_entity);
	}

	template<typename T>
	bool Entity::HasComponent() const
	{
		return m_scene->m_entityRegistry.has<T>(m_entity);
	}
	
	template<typename T, typename ...Args>
	T& Entity::AddComponent(Args && ...args)
	{
		T& component = m_scene->m_entityRegistry.emplace<T>(m_entity, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		m_scene->m_entityRegistry.remove_if_exists<T>(m_entity);
	}
}