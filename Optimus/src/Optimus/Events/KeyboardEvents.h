#pragma once
#include "Event.h"

#include <sstream>

namespace OP
{

	class OPTIMUS_API KeyEvent : public Event
	{
	public:
		virtual ~KeyEvent(){}

		inline int GetKeyCode()const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(KEYBOARD | INPUT)

	protected:
		KeyEvent(int keycode) :m_KeyCode(keycode){}

		int m_KeyCode;
	};

	class OPTIMUS_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount):KeyEvent(keyCode), m_RepeatCount(repeatCount){}
		virtual ~KeyPressedEvent() {}

		inline int GetRepeatCount() { return m_RepeatCount;}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressed Event: " << "KeyCode: " << m_KeyCode << "RepeatCount: " << m_RepeatCount << '\n';
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESS)

	private:
		int m_RepeatCount;
	};


	class OPTIMUS_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) :KeyEvent(keyCode){}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleased Event: " << "KeyCode: " << m_KeyCode <<'\n';
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASED)
	};


	class OPTIMUS_API KeyHoldEvent : public KeyEvent
	{
	public:
		KeyHoldEvent(int keyCode) :KeyEvent(keyCode) {}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyHoldEvent Event: " << "KeyCode: " << m_KeyCode << '\n';
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_HOLD)
	};


}

