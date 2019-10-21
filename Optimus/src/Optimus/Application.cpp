#include "pch.h"

#include "Application.h"
#include "Log.h"

/*TEST*/
#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/SwapChain.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
/*TEST*/


namespace OP
{
#define OP_BIND_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application*   Application::s_Instance = nullptr;

	Application::Application()
	{
		OP_ASSERT(!s_Instance);
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window->SetWindowCallbackFunc(OP_BIND_FN(OnEvent));

		//m_ImguiLayer = new ImguiLayer();
		//PushOverlay(m_ImguiLayer);

		/*TEST*/
		std::unique_ptr<Instance> instance = std::make_unique<Instance>();
		std::unique_ptr<PhysicalDevice> physicalDevice = std::make_unique<PhysicalDevice>(*instance.get());
		std::unique_ptr<Surface> surface = std::make_unique<Surface>(instance.get(), physicalDevice.get());
		std::unique_ptr<LogicalDevice> logicalDevice = std::make_unique<LogicalDevice>(instance.get(), physicalDevice.get(), surface.get());

		std::unique_ptr<SwapChain> swapchain = std::make_unique<SwapChain>(surface.get(), logicalDevice.get());
		std::unique_ptr<RenderPass> renderPass = std::make_unique<RenderPass>(swapchain.get(), logicalDevice.get());
		/*TEST*/
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

			//m_Renderer->drawFrame();
		}
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