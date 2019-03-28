#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/KeyboardEvents.h"

#include "Events/MouseEvents.h"
#include "Events/ApplicationEvents.h"

#include "Log.h"

namespace OP
{
	class OPTIMUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(Event&);

		bool OnWindowClose(Event&);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_isRunning = true;

	};

	Application* CreateApplication();
}

