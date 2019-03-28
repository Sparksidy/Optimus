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

	class MouseButtonEvent : public Event
	{
	public:
		virtual ~MouseButtonEvent() {}

		inline int GetMouseButton()const { return m_Button; }

		EVENT_CLASS_CATEGORY(MOUSE | INPUT)

	protected:
		MouseButtonEvent(int button) :m_Button(button) {}
		int m_Button;										 // 0: leftButton | 1: rightButton | 2: ScrollButton
	};

	class OPTIMUS_API MouseButtonPressed : public MouseButtonEvent
	{
	public:
		MouseButtonPressed(int _button) :MouseButtonEvent(_button) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "Mouse Button Pressed Event: " << "Button:  " << m_Button << '\n';
			return ss.str();
		}
		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class OPTIMUS_API MouseButtonReleased : public MouseButtonEvent
	{
	public:
		MouseButtonReleased(int _button) :MouseButtonEvent(_button) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "Mouse Button Released Event: " << "Button:  " << m_Button << '\n';
			return ss.str();
		}
		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)

	};

	class OPTIMUS_API MouseScroll : public Event
	{
	public:
		MouseScroll(double x, double y) :m_xOffset(x), m_yOffset(y) {}

		inline double GetXOffset()const { return  m_xOffset; }
		inline double GetYOffset()const { return m_yOffset; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "Mouse Scroll Event: " << "XOff:  " << m_xOffset<<"YOff: "<<m_yOffset<< '\n';
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLL)
		EVENT_CLASS_CATEGORY(MOUSE | INPUT)

	private:
		double m_xOffset;
		double m_yOffset;
	};


}
