#include <pch.h>
#include <Optimus.h>

class Sandbox : public OP::Application
{
public:
	Sandbox()
	{

	}
	virtual ~Sandbox()
	{

	}
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}