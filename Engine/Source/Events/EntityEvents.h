#pragma once

#include "Event.h"

#include <typeinfo>

namespace Engine
{
	class Entity;
	class EntityHierarchyComponent;
	class Scene;

	enum class EntityEventType
	{
		EntityCreatedEventType, EntityDestroyedEventType,

		EntityComponentAddedEventType, EntityComponentRemovedEventType, 

		EntityHierarchyChangedEventType
	};

	class EntityCreatedEvent : public Event<EntityEventType>
	{
	public:
		explicit EntityCreatedEvent(const Entity& entity)
			: entity((Entity&)entity) { }

		EVENT_TYPE_CLASS(EntityEventType, EntityCreatedEventType)

		Entity& entity;
	};

	class EntityDestroyedEvent : public Event<EntityEventType>
	{
	public:
		explicit EntityDestroyedEvent(const Entity& entity)
			: entity(entity) { }

		EVENT_TYPE_CLASS(EntityEventType, EntityDestroyedEventType)

		const Entity& entity;
	};

	class EntityHierarchyChangedEvent : public Event<EntityEventType>
	{
	public:
		explicit EntityHierarchyChangedEvent(EntityHierarchyComponent& component)
			: entityHierarchy((EntityHierarchyComponent&)component) { }

		EVENT_TYPE_CLASS(EntityEventType, EntityHierarchyChangedEventType)

		EntityHierarchyComponent& entityHierarchy;
	};

	template<typename TComponent>
	class EntityComponentAddedEvent : public Event<EntityEventType>
	{
	public:
		explicit EntityComponentAddedEvent(Entity& entity, TComponent& component)
			: entity(entity), component(component), componentType(typeid(TComponent)) { }

		/**
		 * Gets the entity. 
		 */
		Entity& GetEntity() { return entity; }
		/**
		 * Get the component. 
		 */
		TComponent& GetComponent() { return component; }

		EVENT_TYPE_CLASS(EntityEventType, EntityComponentAddedEventType);

	private:
		const std::type_info& componentType;
		Entity& entity;
		TComponent& component;
	};

	template<typename TComponent>
	class EntityComponentRemovedEvent : public Event<EntityEventType>
	{
	public:
		explicit EntityComponentRemovedEvent(Entity& entity, TComponent& component)
			: entity(entity), component(component), componentType(typeid(TComponent)) { }

		/**
		 * Gets the entity.
		 */
		Entity& GetEntity() { return entity; }
		/**
		 * Get the component.
		 */
		TComponent& GetComponent() { return component; }

		EVENT_TYPE_CLASS(EntityEventType, EntityComponentRemovedEventType);

	private:
		const std::type_info& componentType;
		Entity& entity;
		TComponent& component;
	};

}