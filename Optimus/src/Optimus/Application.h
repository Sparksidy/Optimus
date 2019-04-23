#pragma once
#include "Core.h"
#include "Window.h"

#include "Optimus/Imgui/ImguiLayer.h"
#include "Optimus/Input/Input.h"
#include "Optimus/Graphics/Renderer.h"

#include "Layer.h"
#include "LayerStack.h"

#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/ApplicationEvents.h"

#include "Log.h"

namespace OP
{
	class OPTIMUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(Event&);
		bool OnWindowClose(Event&);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow()const { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		ImguiLayer* m_ImguiLayer;
		LayerStack m_LayerStack;
		Renderer* m_Renderer;

		bool m_isRunning = true;
		static Application* s_Instance;

	};

	Application* CreateApplication();
}

