#pragma once

#include "Above/Core.h"

namespace Above
{
	//Blocking events, when they occur they immediately must be dealt with
	//e.g: mouse button clicks - the whole application stops and must deal with that mouse press
	
	//TODO: For the future we can create a buffer of events in an event bus
	//and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	/// <summary>
	/// We may want to filter certain events, this helps us with it.
	/// We are using a bitfield so an event can be of multiple categories.
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),		//0b00000001 -> 1  (decimal)
		EventCategoryInput			= BIT(1),		//0b00000010 -> 2  (decimal)
		EventCategoryKeyboard		= BIT(2),		//0b00000100 -> 4  (decimal)
		EventCategoryMouse			= BIT(3),		//0b00001000 -> 8  (decimal)
		EventCategoryMouseButton	= BIT(4),		//0b00010000 -> 16 (decimal)
	};


	//Helper #defines so we can write less when implementing different types of events with only slight differences.

#define	EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	class ABOVE_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		bool Handled = false;

		// Should only be used for debugging purposes.
		virtual const char* GetName() const = 0;

		// Should only be used for debugging purposes.
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// Returns if the event is at least in the indicated category.
		/// </summary>
		/// <param name="category">category to check</param>
		/// <returns></returns>
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename EVENT_TYPE>
		/// <summary>
		/// Standard function which returns bool and takes in a TYPE &.
		/// TYPE in this case could be any event
		/// </summary>
		/// <typeparam name="TYPE"></typeparam>
		using EventFn = std::function<bool(EVENT_TYPE&)>;
		
	public:
		/// <summary>
		/// When we receive an event, we have no idea what event we're receiving.
		/// </summary>
		/// <param name="e"></param>
		EventDispatcher(Event & e) : m_Event(e)
		{
		}

		template<typename EVENT_TYPE>
		bool Dispatch(EventFn<EVENT_TYPE> func)
		{
			if (m_Event.GetEventType() == EVENT_TYPE::GetStaticType())
			{
				//If the event type is the same as the one we're trying to dispatch,
				//call the function we got as parameter.

				m_Event.Handled = func(*(EVENT_TYPE*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event & m_Event;
	};

	inline std::ostream & operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}