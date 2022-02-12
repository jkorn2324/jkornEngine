#pragma once

#include "Event.h"

#include <typeinfo>

namespace Engine
{
	class Entity;
	class EntityHierarchyComponent;
	class Scene;

#define EVENT_TYPE_COMPONENT_CLASS(type, category) template<typename Component>\
	class type : public Event { \
	public: \
		explicit type(const Entity& entity, Component& c) \
			: entity((Entity&)entity), component(c), typeInfo(typeid(Component)) { } \
		const std::type_info& GetComponentType() const { return typeInfo; } \
		static EventType GetStaticEventType() { return EventType::type; }\
		virtual EventType GetEventType() const override { return GetStaticEventType(); }\
		virtual const char* GetName() const override { return #type; }\
		virtual int GetEventCategoryFlags() const override { return (int)EventCategory::category; } \
		static bool StaticIsValid(class Event& event) \
		{ \
			type<Component>& t = (type&)event; \
			return t.typeInfo == typeid(Component); \
		} \
	private: \
		const std::type_info& typeInfo; \
	public: \
		Entity& entity; \
		Component& component; }


	class EntityCreatedEvent : public Event
	{
	public:
		explicit EntityCreatedEvent(const Entity& entity)
			: entity((Entity&)entity) { }

		EVENT_CATEGORY_CLASS(Scene)
		EVENT_TYPE_CLASS(EntityCreatedEvent)

		Entity& entity;
	};

	class EntityDestroyedEvent : public Event
	{
	public:
		explicit EntityDestroyedEvent(const Entity& entity)
			: entity((Entity&)entity) { }

		EVENT_CATEGORY_CLASS(Scene)
		EVENT_TYPE_CLASS(EntityDestroyedEvent)

		Entity& entity;
	};

	class EntityHierarchyChangedEvent : public Event
	{
	public:
		explicit EntityHierarchyChangedEvent(const EntityHierarchyComponent& component)
			: entityHierarchy((EntityHierarchyComponent&)component) { }

		EVENT_TYPE_CLASS(EntityHierarchyChangedEvent)
		EVENT_CATEGORY_CLASS(Scene)

		EntityHierarchyComponent& entityHierarchy;
	};

	EVENT_TYPE_COMPONENT_CLASS(EntityComponentAddedEvent, Scene);
	EVENT_TYPE_COMPONENT_CLASS(EntityComponentRemovedEvent, Scene);
}