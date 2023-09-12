#pragma once

#ifdef AB_PLATFORM_WINDOWS

extern Above::Application* Above::CreateApplication();

int main(int argc, char ** argv)
{
	Above::Log::Init();
	AB_CORE_INFO("Inititialized ABOVE engine");

	AB_PROFILE_BEGIN_SESSION("Startup", "AboveProfiler-Startup.json");
	//CreateApplication() will be the entry point of the application
	auto app = Above::CreateApplication();
	AB_PROFILE_END_SESSION();

	AB_PROFILE_BEGIN_SESSION("Runtime", "AboveProfiler-Runtime.json");
	app->Run();
	AB_PROFILE_END_SESSION();

	AB_PROFILE_BEGIN_SESSION("Shutdown", "AboveProfiler-Shutdown.json");
	delete app;
	AB_PROFILE_END_SESSION();
}

#else
	
#endif