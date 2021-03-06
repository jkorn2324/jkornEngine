#pragma once

#include <string>

namespace Engine
{

	enum class EventType
	{
		WindowClosedEvent, WindowResizedEvent, WindowFocusEvent,

		InputKeyEvent, InputKeyPressedEvent, InputKeyReleasedEvent,
		InputMouseButtonEvent, InputMouseButtonPressedEvent, InputMouseButtonReleasedEvent,
		InputMouseMoveEvent, InputMouseScrollEvent,

		EntityCreatedEvent, EntityDestroyedEvent, EntityHierarchyChangedEvent,
		EntityComponentAddedEvent, EntityComponentRemovedEvent
	};

	enum class EventCategory
	{
		Application,
		Input,
		Scene,
		Editor
	};

#define EVENT_TYPE_CLASS(type) public:\
	static EventType GetStaticEventType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticEventType(); }\
	virtual const char* GetName() const override { return #type; }\
	static bool StaticIsValid(class Event& event) { return true; }
#define EVENT_CATEGORY_CLASS(category) public:	virtual int GetEventCategoryFlags() const override { return (int)EventCategory::category; }
#define BIND_EVENT_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }
#define BIND_STATIC_EVENT_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		virtual int GetEventCategoryFlags() const = 0;

		bool HasEventCategory(EventCategory category)
		{
			return GetEventCategoryFlags() & (int)category;
		}

		virtual bool IsValid() const { return true; }

		bool eventHandled = false;
	};

	class EventDispatcher
	{

	public:
		explicit EventDispatcher(Event& ev)
			: m_event(ev) { }

		template<typename EventType, typename Func>
		bool Invoke(const Func& callback)
		{
			if (m_event.GetEventType() == EventType::GetStaticEventType()
				&& EventType::StaticIsValid(m_event))
			{
				m_event.eventHandled |= callback(static_cast<EventType&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};
}