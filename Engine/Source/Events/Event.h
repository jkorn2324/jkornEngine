#pragma once

#include <string>

namespace Engine
{

	enum class EventType
	{
		WindowClosedEvent, WindowResizedEvent
	};

	enum class EventCategory
	{
		Application
	};

#define EVENT_TYPE_CLASS(type) public:\
	static EventType GetStaticEventType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticEventType(); }\
	virtual const char* GetName() const override { return #type; }
#define EVENT_CATEGORY_CLASS(category) public:	virtual int GetEventCategoryFlags() const override { return (int)EventCategory::category; }
#define BIND_EVENT_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

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
			if (m_event.GetEventType() == EventType::GetStaticEventType())
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