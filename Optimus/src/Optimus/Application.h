#pragma once
#include "Core.h"
#include "Window.h"

#include "Layer.h"
#include "LayerStack.h"

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
		LayerStack m_LayerStack;
		bool m_isRunning = true;

	};

	Application* CreateApplication();
}

