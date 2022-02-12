#pragma once

#include "Event.h"

namespace Engine
{

	class WindowClosedEvent : public Event
	{
	public:
		explicit WindowClosedEvent() = default;

		EVENT_TYPE_CLASS(WindowClosedEvent)
		EVENT_CATEGORY_CLASS(Application)
	};

	class WindowResizedEvent : public Event
	{
	public:
		explicit WindowResizedEvent(std::uint32_t width, std::uint32_t height)
			: Event(), width(width), height(height) { }

		EVENT_TYPE_CLASS(WindowResizedEvent)
		EVENT_CATEGORY_CLASS(Application)

		std::uint32_t width, height;
	};

	class WindowFocusEvent : public Event
	{
	public:
		explicit WindowFocusEvent(bool focus)
			: Event(), focused(focus) { }

		EVENT_TYPE_CLASS(WindowFocusEvent)
		EVENT_CATEGORY_CLASS(Application)

		bool focused;
	};
}