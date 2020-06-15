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

		virtual bool Initialize();

		virtual void Run();

		void Unload();

		void DeAllocateSystems();

		void OnEvent(Event&);
		bool OnWindowClose(Event&);
		bool OnWindowResize(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ISystem* GetSystem(const std::string& system);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()const { return *m_Window; }
		inline ImguiLayer& GetImGUILayer()const { return *m_ImguiLayer; }
		inline bool IsWindowResized()const { return m_windowResized; }

	private:
		std::unique_ptr<Window> m_Window;

		//Stores the systems with their names
		std::map<std::string, ISystem*> m_Systems;

		//TODO: Layers: Probably Scene class should hold this.
		ImguiLayer* m_ImguiLayer;
		LayerStack m_LayerStack;

		bool m_isRunning = true;
		bool m_windowResized = false;
		static Application* s_Instance;

		float currTime = 0.0f;
	};

	Application* CreateApplication();
}

