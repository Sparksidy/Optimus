#include "Application.h"

#include "Optimus/Log.h"

namespace OP
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		OP::KeyPressedEvent e(12, 2);
		if (e.isInCategory(EVENT_CATEGORY::KEYBOARD))
		{
			OP_TRACE(e);
		}
		while (true);
	}
}