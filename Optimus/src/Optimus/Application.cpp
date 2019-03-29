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
		while (m_isRunning)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->Update();

		}
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dis(e);
		dis.Dispatch<WindowCloseEvent>(OP_BIND_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*it)->OnEvent(e);
			--it;
			if (e.Handled())
				break;
		}

		OP_CORE_INFO("{0}", e);
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_isRunning = false;
		return true;
	}
}