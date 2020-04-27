#pragma once
#include <Optimus/Core.h>
#include <Optimus/Window.h>

#include <Optimus/Imgui/ImguiLayer.h>
#include <Optimus/Input/Input.h>
#include <Optimus/Graphics/Core/Graphics.h>

#include "Layer.h"
#include "LayerStack.h"

#include <Optimus/Events/Event.h>
#include <Optimus/Events/KeyboardEvents.h>
#include <Optimus/Events/MouseEvents.h>
#include <Optimus/Events/ApplicationEvents.h>

#include <Optimus/Log.h>

namespace OP
{
	class OPTIMUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void AllocateSystems();

		bool Initialize();

		virtual void Run();

		void Unload();

		void DeAllocateSystems();

		void OnEvent(Event&);
		bool OnWindowClose(Event&);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ISystem* GetSystem(const std::string& system);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()const { return *m_Window; }
		inline ImguiLayer& GetImGUILayer()const { return *m_ImguiLayer; }

	private:

		std::unique_ptr<Window> m_Window;

		//Stores the systems with their names
		std::map<std::string, ISystem*> m_Systems;

		//TODO: Layers: Probably Scene class should hold this.
		ImguiLayer* m_ImguiLayer;
		LayerStack m_LayerStack;

		bool m_isRunning = true;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

