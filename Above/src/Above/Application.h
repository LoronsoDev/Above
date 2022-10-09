#pragma once

#include "Core.h"

namespace Above
{
	class ABOVE_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();

	};


	// To be defined in CLIENT
	Application* CreateApplication();

}

