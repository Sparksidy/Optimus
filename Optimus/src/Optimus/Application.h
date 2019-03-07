#pragma once
#include "Core.h"

namespace OP
{
	class OPTIMUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run() = 0;

	};

	Application* CreateApplication();
}

