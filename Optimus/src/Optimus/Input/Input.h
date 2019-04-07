#pragma once
#include <GLFW/glfw3.h>
#include "Optimus/Core.h"
#include "Optimus/KeyCodes.h"
#include "Optimus/Application.h"

namespace OP
{
	class OPTIMUS_API Input
	{
	public:
		Input();
		~Input();

		bool IsKeyPressed(const int&)const;
		bool IsMouseButtonPressed(const int&)const;

		std::pair<double, double> GetMousePos()const;
		double GetMousePosX()const;
		double GetMousePosY()const;

		inline static Input& Get() { return *s_Instance; }

	private:
		static Input* s_Instance;

		friend class Window;
	};
}
