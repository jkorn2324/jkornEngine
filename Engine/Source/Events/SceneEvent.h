#pragma once

#include "Event.h"
#include "Entity.h"

namespace Engine
{
	class EntityCreatedEvent : public Event
	{
	public:
		explicit EntityCreatedEvent(const Entity& entity)
			: entity(entity) { }

		EVENT_CATEGORY_CLASS(Scene)
		EVENT_TYPE_CLASS(EntityCreatedEvent)

		Entity entity;
	};

	class EntityDestroyedEvent : public Event
	{
	public:
		explicit EntityDestroyedEvent(const Entity& entity)
			: entity(entity) { }

		EVENT_CATEGORY_CLASS(Scene)
		EVENT_TYPE_CLASS(EntityDestroyedEvent)

		Entity entity;
	};
}