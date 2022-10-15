#include "abpch.h"

#include "Application.h"

#include "Above/Events/ApplicationEvent.h"
#include "Above/Log.h"

namespace Above
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}