#pragma once
#include "Event.h"

namespace OP
{
	class OPTIMUS_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) :m_CursorX(x), m_CursorY(y) {}
		~MouseMoveEvent() {}

		inline float GetX()const { return m_CursorX; }
		inline float GetY()const { return m_CursorY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "Mouse Move Event: " << "X: " << m_CursorX << " Y: " << m_CursorY << '\n';
			return ss.str();
		}
		

		EVENT_CLASS_TYPE(MOUSE_MOVE)
		EVENT_CLASS_CATEGORY(MOUSE | INPUT)

	private:
		float m_CursorX, m_CursorY;
	};



}
