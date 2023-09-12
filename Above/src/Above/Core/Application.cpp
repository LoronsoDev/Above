#include "abpch.h"

#include "Application.h"
#include "Above/Core/Log.h"
#include "Above/Core/Input.h"
#include "Above/Renderer/Buffer.h"

//temp
#include <glad/glad.h>

#include <Above/Renderer/Renderer.h>

#include "Above/Renderer/RenderCommand.h"

#include <GLFW/glfw3.h>

namespace Above
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1));

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		AB_PROFILE_FUNCTION();

		AB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		AB_PROFILE_FUNCTION();

	}

	void Application::PushLayer(Layer* layer)
	{
		AB_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		AB_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		AB_PROFILE_FUNCTION();

		while (m_Running)
		{
			AB_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime(); //This should be platform dependent ( Platform::GetTime() )
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
			{
				{
					AB_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

			}

			m_ImGuiLayer->Begin();
			{
				AB_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		AB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		AB_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
