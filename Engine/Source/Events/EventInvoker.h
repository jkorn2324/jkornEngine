#pragma once

#include <functional>

namespace Engine
{
	class IEvent;

	// The Event Function Declaration.
	using EventFunc = std::function<void(IEvent&)>;

	/**
	 * The event invoker function. 
	 */
	class EventInvoker
	{
	public:
		EventInvoker()
			: m_eventFunc(nullptr) { }

		EventInvoker(const EventFunc& func)
			: m_eventFunc(func) { }

		/**
		 * Sets the event function.
		 */
		void SetEventFunc(const EventFunc& func)
		{
			m_eventFunc = func;
		}

		template<typename TEvent>
		void Invoke(TEvent& inEvent)
		{
			static_assert(std::is_base_of<IEvent, TEvent>::value, "The event must be inherited.");
			if (m_eventFunc)
			{
				m_eventFunc(inEvent);
			}
		}

	private:
		EventFunc m_eventFunc;

	public:
		/**
		 * Gets a global static event invoker. 
		 */
		static EventInvoker& Global()
		{
			static EventInvoker s_eventInvoker;
			return s_eventInvoker;
		}
	};
}