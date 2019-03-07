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
	void Run()
	{
		while (true);
	}
};

OP::Application* OP::CreateApplication()
{
	return new Sandbox();
}