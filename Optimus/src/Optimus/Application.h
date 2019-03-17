#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Events/KeyboardEvents.h"

namespace OP
{
	class OPTIMUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

	};

	Application* CreateApplication();
}

