#include "pch.h"

#include "Application.h"
#include "Log.h"

namespace OP
{
#define OP_BIND_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::make_unique<Window>();
		m_Window->SetWindowCallbackFunc(OP_BIND_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (isRunning)
		{
			glClearColor(1.0, 0.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->Update();
		}
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dis(e);
		dis.Dispatch<MouseMoveEvent>(OP_BIND_FN(OnMouseMove));
	}
	bool Application::OnMouseMove(Event & e)
	{
		OP_CORE_INFO("{0}", e);
		return true;
	}
}