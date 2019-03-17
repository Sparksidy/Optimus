#include "pch.h"
#include "Window.h"
#include "Log.h"

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

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
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
