#pragma once
#include "Event.h"

namespace OP
{
	class OPTIMUS_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int w, int h) :m_Width(w), m_Height(h) {}
		~WindowResizeEvent() {}

		inline int GetWidth()const { return m_Width; }
		inline int GetHeight()const { return m_Height; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "Window Resize Event: " << "Width: " << m_Width << " Height: " << m_Height << '\n';
			return ss.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(APPLICATION)

	private:
		int m_Width;
		int m_Height;
	};

	class OPTIMUS_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(APPLICATION)

	};

}

