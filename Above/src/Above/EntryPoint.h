#pragma once

#include <stdio.h>

#ifdef AB_PLATFORM_WINDOWS

extern Above::Application* Above::CreateApplication();

int main(int argc, char ** argv)
{
	printf("ABOVE ENGINE STARTED...");
	auto app = Above::CreateApplication();
	app->Run();
	delete app;
}

#else
	
#endif