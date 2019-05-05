#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/ApplicationEvents.h"


namespace OP
{
	struct WindowProps
	{
		std::string m_Title;
		int m_Width;
		int m_Height;

		WindowProps(const std::string& title = "Optimus Engine",
			int width = 1080,
			int height = 720) :m_Title(title), m_Width(width), m_Height(height) {}
	};


	class Instance;

	class OPTIMUS_API Window
	{
		public:
			using EventCallbackFn = std::function<void(Event&)>;

		public:
			Window(const WindowProps& props = WindowProps());
			~Window() { DestroyWindow();}

			inline void SetWindowCallbackFunc(const EventCallbackFn& eventCallback) { m_Data.EventCallback = eventCallback; }
			inline int GetWindowWidth()const { return m_Data.width; }
			inline int GetWindowHeight()const { return m_Data.height;}
			inline GLFWwindow* GetWindowPointer()const { return m_Window; }

			void Update();

			VkResult CreateSurface(const VkInstance& instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface)const;

		private:
			void DestroyWindow();
			void InitWindow(const WindowProps& props);

		private:

			struct WindowData
			{
				int width;
				int height;
				std::string title;

				EventCallbackFn EventCallback;
			};
			WindowData m_Data;
			GLFWwindow* m_Window;

			static bool s_isGLFWInitialized;
	};
}