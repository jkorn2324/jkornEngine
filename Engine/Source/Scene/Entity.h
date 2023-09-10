#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "EnttUtility.h"

#include "SceneEvent.h"
#include "Scene.h"

namespace Engine
{
	using EventFunc = std::function<void(IEvent&)>;

	/**
	 * The Entity class. (Defines what consists of an entity in the game)
	 */
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
		return Entt::GetComponent<T>(m_entity, m_scene->m_entityRegistry);
	}

	template<typename T>
	T& Entity::GetComponent()
	{
		return Entt::GetComponent<T>(m_entity, m_scene->m_entityRegistry);
	}

	template<typename T>
	bool Entity::HasComponent() const
	{
		return Entt::HasComponent<T>(m_entity, m_scene->m_entityRegistry);
	}

	template<typename T, typename ...Args>
	T& Entity::AddComponent(Args && ...args)
	{
		DebugAssert(IsValid(), "Entity must be valid for it to be added as a component.");
		T& component = Entt::AddComponent<T>(m_entity, m_scene->m_entityRegistry, std::forward<Args>(args)...);
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
		Entt::RemoveComponent<T>(m_entity, m_scene->m_entityRegistry,
			[](T& component, Entity& entity) -> void {
				if (s_componentEventFunc != nullptr)
				{
					EntityComponentRemovedEvent<T> event(entity, component);
					s_componentEventFunc(event);
				}
			}, *this);
#if 0
		T& component = GetComponent<T>();
		if (s_componentEventFunc != nullptr)
		{
			EntityComponentRemovedEvent<T> event(*this, component);
			s_componentEventFunc(event);
		}
		m_scene->m_entityRegistry.remove_if_exists<T>(m_entity);
#endif
	}
}