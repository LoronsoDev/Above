#pragma once

#include "Core.h"

#include "Window.h"
#include "Above/LayerStack.h"
#include "Above/Events/Event.h"
#include "Above/Events/ApplicationEvent.h"

#include "Above/ImGui/ImGuiLayer.h"

//TEMP
#include "Above/Renderer/Shader.h"
#include "Above/Renderer/Buffer.h"
#include "Above/Renderer/VertexArray.h"

#include <Above/Renderer/OrthographicCamera.h>

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

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};


	// To be defined in CLIENT
	Application* CreateApplication();
}

