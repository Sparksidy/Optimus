#include "pch.h"
#include "Input.h"

namespace OP
{
	Input* Input::s_Instance = nullptr;

	Input::Input()
	{
		OP_ASSERT(!s_Instance);
		s_Instance = this;
	}
	Input::~Input()
	{
		s_Instance = nullptr;
	}
	bool Input::IsKeyPressed(const int& key) const
	{
		Window& window = Application::Get().GetWindow();
		int status = glfwGetKey(window.GetWindowPointer(), key);

		return true ? status == GLFW_PRESS || status == GLFW_REPEAT : false;
	}
	bool Input::IsMouseButtonPressed(const int& button) const
	{
		Window& window = Application::Get().GetWindow();
		int status = glfwGetMouseButton(window.GetWindowPointer(), button);

		return true ? status == GLFW_PRESS : false;
	}
	std::pair<double, double> Input::GetMousePos() const
	{
		double xPos, yPos;
		Window& window = Application::Get().GetWindow();
		glfwGetCursorPos(window.GetWindowPointer(), &xPos, &yPos);

		return std::make_pair(xPos, yPos);
	}
	double Input::GetMousePosX() const
	{
		auto [x, y] = GetMousePos();
		return x;
	}
	double Input::GetMousePosY() const
	{
		auto[x, y] = GetMousePos();
		return y;
	}

}

