#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Above
{
	/// <summary>
	/// Defines the common behaviour for all apps.
	/// </summary>
	class ABOVE_API Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};


	// To be defined in CLIENT
	Application* CreateApplication();
}

