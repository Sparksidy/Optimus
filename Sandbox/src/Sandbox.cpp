#include <pch.h>
#include <Optimus.h>

#include "MainRenderer.h"

class Sandbox : public OP::Application
{
public:
	Sandbox()
	{
	}

	virtual bool Initialize() override
	{
		//Set Renderer
		dynamic_cast<OP::Graphics*>(Application::Get().GetSystem("Graphics"))->SetRenderer(std::make_unique<MainRenderer>());

		//Set Scene

		return true;
	}

	virtual ~Sandbox()
	{
		OP_CORE_INFO("Destructing Sandbox");
	}
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}