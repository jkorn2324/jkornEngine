#pragma once

#include "Event.h"

namespace Engine
{
	/**
	 * The window event type. 
	 */
	enum class WindowEventType
	{
		WindowClosedEvent, WindowResizedEvent, WindowFocusEvent
	};

	/**
	 * The window closed event. 
	 */
	class WindowClosedEvent : public Event<WindowEventType>
	{
	public:
		explicit WindowClosedEvent() = default;

		EVENT_TYPE_CLASS(WindowEventType, WindowClosedEvent)
	};

	/**
	 * The window resized event. 
	 */
	class WindowResizedEvent : public Event<WindowEventType>
	{
	public:
		explicit WindowResizedEvent(std::uint32_t width, std::uint32_t height)
			: Event(), width(width), height(height) { }

		EVENT_TYPE_CLASS(WindowEventType, WindowResizedEvent)

		std::uint32_t width, height;
	};

	/**
	 * The window focused event. 
	 */
	class WindowFocusEvent : public Event<WindowEventType>
	{
	public:
		explicit WindowFocusEvent(bool focus)
			: Event(), focused(focus) { }

		EVENT_TYPE_CLASS(WindowEventType, WindowFocusEvent)

		bool focused;
	};
}