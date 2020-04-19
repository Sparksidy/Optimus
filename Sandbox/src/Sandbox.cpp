#include <pch.h>
#include <Optimus.h>

#include "MainRenderer.h"

class Sandbox : public OP::Application
{
public:
	Sandbox()
	{
		Start();
	}
	
	void Start()
	{
		//Set the Renderer for the application
		auto graphics = dynamic_cast<OP::Graphics*>(Application::Get().GetSystem("Graphics"));
		graphics->SetRenderer(std::make_unique<MainRenderer>());
	}

	virtual ~Sandbox()
	{

	}
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}