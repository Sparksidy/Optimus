#pragma once

#ifdef OP_PLATFORM_WINDOWS

extern OP::Application* OP::CreateApplication();

int main(int argc, char** argv)
{
	OP::Log::Init();

	auto app = OP::CreateApplication();
	app->Run();
	delete(app);
}

#endif