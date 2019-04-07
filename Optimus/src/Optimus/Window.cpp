#include "pch.h"
#include "Window.h"
#include "Log.h"

#include <glad/glad.h>

bool OP::Window::s_isGLFWInitialized = false;

OP::Window::Window(const WindowProps & props)
{
	InitWindow(props);
}


void OP::Window::InitWindow(const WindowProps& props)
{
	m_Data.title = props.m_Title;
	m_Data.width = props.m_Width;
	m_Data.height = props.m_Height;

	if (!s_isGLFWInitialized)
	{
		if (!glfwInit())
			OP_FATAL("Unable to initialize GLFW");

		s_isGLFWInitialized = true;
	}

	OP_CORE_INFO("Creating window... {0} ({1}, {2})", props.m_Title, props.m_Width, props.m_Height);

	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_Window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
	{
		OP_FATAL("Unable to initialize GLAD");
	}

	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key, 0);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(key);
				data.EventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyHoldEvent e(key);
				data.EventCallback(e);
				break;
			}
		}
	}
	);

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		KeyTypedEvent e(codepoint);
		data.EventCallback(e);

		OP_CORE_INFO("Character pressed:  {0}", codepoint);
	}
	);

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMove e(x, y);
		data.EventCallback(e);
	}
	);

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressed e(button);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleased e(button);
				data.EventCallback(e);
				break;

			}
		}

	}
	);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScroll e(xOffset, yOffset);
		data.EventCallback(e);
	}
	);

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;

		WindowResizeEvent e(width, height);
		data.EventCallback(e);
	}
	);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
		OP::Window::WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowCloseEvent e;
		data.EventCallback(e);
	}
	);


}

void OP::Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void OP::Window::DestroyWindow()
{
	glfwDestroyWindow(m_Window);
}
