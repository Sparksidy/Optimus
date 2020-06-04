#include <pch.h>
#include <Optimus.h>

#include "MainRenderer.h"
#include "Scene1.h"

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
		dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->SetScene(std::make_unique<Scene1>());

		return true;
	}

	virtual ~Sandbox()
	{
		OP_CORE_INFO("Destructing Sandbox");
	}

private:
	
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}