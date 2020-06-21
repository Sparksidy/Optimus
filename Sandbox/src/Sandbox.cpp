#include <pch.h>
#include <Optimus.h>

#include "MainRenderer.h"
#include <Optimus/Systems/SceneManager.h>

class Sandbox : public OP::Application
{
public:
	Sandbox()
	{
		OP_CORE_INFO("Constructing Sandbox");
	}

	virtual bool Initialize() override
	{
		dynamic_cast<OP::Graphics*>(GetSystem("Graphics"))->SetRenderer(std::make_unique<MainRenderer>());

		dynamic_cast<OP::SceneManager*>(GetSystem("SceneManager"))->LoadScenes("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Sandbox\\Resources\\Levels");

		OP_CORE_INFO("Sandbox Initialize!");

		OP::Application::Initialize();

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