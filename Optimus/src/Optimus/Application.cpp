#include "pch.h"

#include "Application.h"
#include "Log.h"
#include <Optimus/Graphics/Devices/LogicalDevice.h>

#include <Optimus/ISystem.h>


namespace OP
{
#define OP_BIND_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application*   Application::s_Instance = nullptr;

	Application::Application()
	{
		OP_ASSERT(!s_Instance);
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window ->SetWindowCallbackFunc(OP_BIND_FN(OnEvent));

		//m_ImguiLayer = new ImguiLayer();
		//PushOverlay(m_ImguiLayer);

		//m_Graphics = std::make_unique<Graphics>();
	}

	void Application::AllocateSystems()
	{
		ISystem* graphics = new Graphics();
		if (graphics)
			m_Systems[graphics->GetName()] = graphics;
	}

	bool Application::Initialize()
	{
		for (auto system : m_Systems)
			if (!system.second->Initialize())
				return false;

		return true;
	}

	Application::~Application()
	{
		OP_CORE_INFO("Destroying Application");
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

			for (auto system : m_Systems)
				system.second->Update();
		}
	}

	void Application::Unload()
	{
		for (auto system : m_Systems)
			system.second->Unload();
	}

	void Application::DeAllocateSystems()
	{
		for (auto system : m_Systems)
			delete system.second;

		m_Systems.clear();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dis(e);
		dis.Dispatch<WindowCloseEvent>(OP_BIND_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_isRunning = false;
		return true;
	}
	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}