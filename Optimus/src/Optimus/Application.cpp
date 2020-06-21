#include "pch.h"

#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>

#include <Optimus/ISystem.h>
#include <Optimus/Systems/GameObjectManager.h>
#include <Optimus/Systems/SceneManager.h>

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

		m_ImguiLayer = new ImguiLayer();
	}

	void Application::AllocateSystems()
	{
		ISystem* graphics = new Graphics();
		if (graphics)
			m_Systems[graphics->GetName()] = graphics;

		OP_CORE_INFO("Graphics System has been created");

		ISystem* gameObjectManager = new GameObjectManager();
		if (gameObjectManager)
			m_Systems[gameObjectManager->GetName()] = gameObjectManager;

		OP_CORE_INFO("Game Object Manager has been created");


		ISystem* sceneManager = new SceneManager();
		if (sceneManager)
			m_Systems[sceneManager->GetName()] = sceneManager;

		OP_CORE_INFO("Scene Manager has been created");
	}

	bool Application::Initialize()
	{
		for (auto system : m_Systems)
			if (!system.second->Initialize())
				return false;

		PushOverlay(m_ImguiLayer);

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
			float time = (float)glfwGetTime();
			float deltaTime = currTime > 0.0f ? time - currTime : (1.0f / 60.0f);
			currTime = time;

			//OP_CORE_INFO("Time: {0}", deltaTime);

			//Update Windows
			m_Window->Update();


			//Update the all Layers
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			
			//Update all the systems
			for (auto system : m_Systems)
				system.second->Update();

		}
	}

	void Application::Unload()
	{
		//Detach the layers first
		for (auto layer : m_LayerStack)
			layer->OnDetach();

		//Unload all the systems
		for (auto system : m_Systems)
			system.second->Unload();
	}

	void Application::DeAllocateSystems()
	{
		//Wait for a queue to become idle before freeing resources
		OP_VULKAN_ASSERT(vkQueueWaitIdle, GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue());

		delete m_Systems["SceneManager"];
		delete m_Systems["GameObjectManager"];
		delete m_Systems["Graphics"];

		m_Systems.clear();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dis(e);

		dis.Dispatch<WindowCloseEvent>(OP_BIND_FN(OnWindowClose));
		dis.Dispatch<WindowResizeEvent>(OP_BIND_FN(OnWindowResize));

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

	bool Application::OnWindowResize(Event& e)
	{
		m_windowResized = true;
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

	inline ISystem* Application::GetSystem(const std::string& system)
	{
		if (m_Systems.count(system))
			return m_Systems[system];

		return nullptr;
	}

}