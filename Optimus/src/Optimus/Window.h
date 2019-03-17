#pragma once
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Events/Event.h"

namespace OP
{
	struct WindowProps
	{
		std::string m_Title;
		int m_Width;
		int m_Height;

		WindowProps(const std::string& title = "Optimus Engine",
			int width = 800,
			int height = 600) :m_Title(title), m_Width(width), m_Height(height) {}
	};


	class OPTIMUS_API Window
	{
		public:
			Window(const WindowProps& props = WindowProps());
			~Window() { DestroyWindow();}

			inline int GetWindowWidth()const { return m_Data.width; }
			inline int GetWindowHeight()const { return m_Data.height;}
			void Update();
		private:
			void DestroyWindow();
			void InitWindow(const WindowProps& props);

			inline GLFWwindow* GetWindowPointer()const { return m_Window; }
		private:

			struct WindowData
			{
				int width;
				int height;
				std::string title;
				using EventCallbackFun = std::function<void(Event&)>;
			};
			WindowData m_Data;
			GLFWwindow* m_Window;

			static bool s_isGLFWInitialized;
	};
}