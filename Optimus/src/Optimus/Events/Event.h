#pragma once
#include "Optimus/Core.h"

namespace OP
{
	enum class EVENT_TYPE
	{
		 NONE = 0,
		 KEY_PRESS, KEY_HOLD, KEY_RELEASED,
		 MOUSE_MOVE, MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLL
	};

	enum EVENT_CATEGORY
	{
		NONE = 0,
		KEYBOARD	=	BIT(0),
		MOUSE		=	BIT(1),
		APPLICATION	=	BIT(2),
		INPUT		=	BIT(3)
	};

#define EVENT_CLASS_TYPE(type) static EVENT_TYPE GetStaticType(){return EVENT_TYPE::##type;}\
						  virtual EVENT_TYPE GetEventType()const override{return GetStaticType();}\
						  virtual const char* GetName()const override{return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategory()const override{return category;}

	class OPTIMUS_API Event
	{
	public:
		bool m_Handled = false;

		Event() {};
		virtual ~Event() {}

		virtual EVENT_TYPE GetEventType()const = 0;
		virtual const char* GetName()const = 0;
		virtual int GetEventCategory()const = 0;

		inline bool isInCategory(EVENT_CATEGORY category) { return  category & GetEventCategory(); }

		virtual std::string ToString()const { return GetName(); }
	};

	class EventDispatcher
	{
	public:
		template <typename T>
		using EventFunc = std::function<bool(T&)>;

		EventDispatcher(Event& event) :m_Event(event) {}

		template <typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Handled = func(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		bool m_Handled = false;
		Event& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}