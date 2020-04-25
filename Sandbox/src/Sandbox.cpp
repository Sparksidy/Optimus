#include <pch.h>
#include <Optimus.h>

#include "MainRenderer.h"

class Sandbox : public OP::Application
{
public:
	Sandbox()
	{
	}
	
	void Run() override
	{
		if (!isInitialized)
		{
			//Set the Renderer for the application
			dynamic_cast<OP::Graphics*>(Application::Get().GetSystem("Graphics"))->SetRenderer(std::make_unique<MainRenderer>());

			isInitialized = true;
		}
		
		OP::Application::Run();
	}

	virtual ~Sandbox()
	{

	}

private:
	bool isInitialized = false;
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}