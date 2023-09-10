#include "Sandbox2D.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <Platform/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D()
	:
	Layer("Sandbox2D"),
	m_CameraController(1280.0f/720.0f)
{

}

void Sandbox2D::OnAttach()
{ 
	
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Above::Timestep timestep)
{
	m_CameraController.OnUpdate(timestep);

	//render
	Above::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
	Above::RenderCommand::Clear();

	Above::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Above::Renderer2D::DrawQuad({0.f, 0.f}, {1.f, 1.f}, {0.8f, -2.f, -3.f, 1.f});
	Above::Renderer2D::EndScene();

	//std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	//Above::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Above::Event& e)
{
	m_CameraController.OnEvent(e);
}
