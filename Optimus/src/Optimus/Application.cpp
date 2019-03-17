#include "pch.h"

#include "Application.h"
#include "Log.h"

namespace OP
{
	Application::Application()
	{
		m_Window = std::make_unique<Window>();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (isRunning)
		{
			m_Window->Update();
		}
	}
}