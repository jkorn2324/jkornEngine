#pragma once

#include <vector>
#include <memory>
#include <utility>

#include <entt.hpp>

#include "SceneEvent.h"
#include "Scene.h"

namespace Engine
{
	using EventFunc = std::function<void(Event&)>;

	class Entity
	{
	public:
		explicit Entity() = default;
		explicit Entity(uint32_t id, class Scene* scene);
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

		const entt::entity& GetID() const { return m_entity; }

		const class Scene& GetScene() const { return *m_scene; }

		/**
		 * Casted to a bool so that we can check for if its valid.
		 */
		explicit operator bool() const
		{
			return IsValid();
		}

		friend bool operator==(const Entity& a, const Entity& b)
		{
			return a.m_entity == b.m_entity;
		}

		friend bool operator!=(const Entity& a, const Entity& b)
		{
			return a.m_entity != b.m_entity;
		}

		friend void CopyEntity(const Entity& from, Entity& to, bool copyName = true);

	private:
		static void BindEventFunc(const EventFunc& func);

	private:
		class Scene* m_scene = nullptr;
		entt::entity m_entity{ entt::null };

		static EventFunc s_componentEventFunc;

		friend class Scene;
		friend class Application;

	public:
		static const Entity None;
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
		if (s_componentEventFunc != nullptr)
		{
			EntityComponentAddedEvent<T> event(*this, component);
			s_componentEventFunc(event);
		}
		return component;
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		if (!HasComponent<T>())
		{
			return;
		}
		T& component = GetComponent<T>();
		if (s_componentEventFunc != nullptr)
		{
			EntityComponentRemovedEvent<T> event(*this, component);
			s_componentEventFunc(event);
		}

		m_scene->m_entityRegistry.remove_if_exists<T>(m_entity);
	}
}