#include "Sandbox2D.h"

#include "imgui/imgui.h"
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
	m_CheckerboardTexture = Above::Texture2D::Create("assets/textures/checkerboard.png");
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

	Above::Renderer2D::DrawQuad({-1.f, 0.f}, 45.f, {0.8f, 0.8f}, {0.8f, .2f, .3f, 1.f});
	Above::Renderer2D::DrawQuad({0.5f, -0.5f}, 0.f, {0.5f, 0.75f}, {0.2f, .3f, .8f, 1.f});
	Above::Renderer2D::DrawQuad({0.2f, -0.5f, -0.1f}, 0.f, {10.0f, 10.0f}, m_CheckerboardTexture);
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
