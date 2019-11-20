#pragma once

#ifdef OP_PLATFORM_WINDOWS

extern OP::Application* OP::CreateApplication();

int main(int argc, char** argv)
{
	OP::Log::Init();

	auto app = OP::CreateApplication();
	if (app)
	{
		app->AllocateSystems();

		if(app->Initialize())
			app->Run();

		app->Unload();

		app->DeAllocateSystems();
	}
	delete(app);
}

#endif