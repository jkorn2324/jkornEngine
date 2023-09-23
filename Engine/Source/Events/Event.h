#pragma once

#include <string>

namespace Engine
{

	// The macros for defining an event type.
#define EVENT_TYPE_CLASS(TEnumType, EnumValue) public:\
	static TEnumType GetStaticEventType() { return TEnumType::EnumValue; }\
	virtual TEnumType GetEventType() const override { return GetStaticEventType(); }\
	virtual const char* GetName() const override { return #EnumValue; }
#define BIND_EVENT_FUNCTION(func) [this](auto&&...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }
#define BIND_STATIC_EVENT_FUNCTION(func) [](auto&&...args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }

	/**
	 * Interface for an event.
	 */
	class IEvent
	{
	public:
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		virtual bool IsValid() const { return true; }

		template<typename TEventClass>
		static bool StaticIsValid(class IEvent& event)
		{
			TEventClass* eventClass = dynamic_cast<TEventClass*>(&event);
			if (eventClass != nullptr)
			{
				return eventClass->IsValid();
			}
			return false;
		}
	};

	template<typename TEventType>
	class Event : public IEvent
	{
		static_assert(std::is_enum<TEventType>::value, "TEventType must be constrained to an enum or enum class.");

	public:
		virtual ~Event() = default;

		virtual TEventType GetEventType() const = 0;

		bool eventHandled = false;
	};

	/**
	 * A templated event dispatcher. Used for invoking a variety of events.
	 */
	class EventDispatcher
	{
	public:
		explicit EventDispatcher(IEvent& ev)
			: m_event(&ev) { }

		template<typename TEventTypeParam, typename TEventType, typename Func>
		bool Invoke(const Func& callback)
		{
			// Asserts the the event type param is an enum & that the
			// event that is searched is a base of an Event<TEventTypeParam>
			static_assert(std::is_enum<TEventTypeParam>::value,
				"TEventType must be constrained to an enum or enum class.");
			static_assert(std::is_base_of<Event<TEventTypeParam>, TEventType>::value,
				"The TEventType must derive from an Event of type Event<TEventTypeParam>.");

			if (m_event == nullptr)
			{
				return false;
			}

			Event<TEventTypeParam>* Casted = static_cast<Event<TEventTypeParam>*>(m_event);
			if (Casted->GetEventType() == TEventType::GetStaticEventType()
				&& IEvent::StaticIsValid<TEventType>(*m_event))
			{
				Casted->eventHandled |= callback(static_cast<TEventType&>(*m_event));
				return true;
			}
			return false;
		}

	private:
		IEvent* m_event;
	};
}