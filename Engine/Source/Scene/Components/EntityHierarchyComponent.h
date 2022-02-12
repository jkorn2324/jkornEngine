#pragma once

#include "Entity.h"
#include <vector>

namespace Engine
{
	// Used to store relationships between entities.
	class EntityHierarchyComponent
	{
	public:
		explicit EntityHierarchyComponent()
			: m_parentEntity(), m_children(), m_owner() { }
		explicit EntityHierarchyComponent(const Entity& owner)
			: m_parentEntity(), m_children(), m_owner(owner) { }

		void SetParent(const Entity& entity);
		bool HasParent() const { return m_parentEntity.IsValid(); }

		bool HasChildren() const { return m_children.size() > 0; }
		bool ContainsChild(const Entity& e);
		const std::vector<Entity>& GetChildren() const { return m_children; }

		const Entity& GetOwner() const { return m_owner; }

	private:
		static void BindEventFunc(const EventFunc& eventFunc);

	private:
		std::vector<Entity> m_children;
		Entity m_parentEntity;
		Entity m_owner;

		friend class SceneSerializer;
		friend class Application;
	};
}