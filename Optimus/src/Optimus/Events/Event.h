#pragma once
#include "Optimus/Core.h"
#include <string>

namespace OP
{
	enum class EVENT_TYPE
	{
		 NONE = 0,
		 KEY_PRESS, KEY_HOLD, KEY_RELEASED
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
		Event() {};
		virtual ~Event() {}

		virtual EVENT_TYPE GetEventType()const = 0;
		virtual const char* GetName()const = 0;
		virtual int GetEventCategory()const = 0;

		inline bool isInCategory(EVENT_CATEGORY category) { return  category & GetEventCategory(); }

		virtual std::string ToString()const { return GetName(); }
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}