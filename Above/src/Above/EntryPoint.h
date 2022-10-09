#pragma once

#include <stdio.h>

#ifdef AB_PLATFORM_WINDOWS

extern Above::Application* Above::CreateApplication();

int main(int argc, char ** argv)
{
	Above::Log::Init();
	AB_CORE_INFO("Inititialized ABOVE engine");

	AB_CORE_WARN("Initialized core log!");
	AB_WARN("Initialized client log!");

	auto app = Above::CreateApplication();
	app->Run();
	delete app;
}

#else
	
#endif