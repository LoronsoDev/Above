#pragma once

#include "Event.h"

namespace Above
{
	class ABOVE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned width, unsigned height) : m_Width(width), m_Height(height) {}
		~WindowResizeEvent() = default;

		inline unsigned GetWidth()  { return m_Width; }
		inline unsigned GetHeight() { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: [" << m_Width << " x " << m_Height << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned m_Width, m_Height;
	};

	class ABOVE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() = default;

		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class ABOVE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}
		~AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
	class ABOVE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}
		~AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class ABOVE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}
		~AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}