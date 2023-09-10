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
	m_SquareVA = Above::VertexArray::Create();

	float squareVertices[3 * 4] =
	{
		-.5f, -.5f, 0.0f,
		 .5f, -.5f, 0.0f,
		 .5f,  .5f, 0.0f,
		-.5f,  .5f, 0.0f
	};

	Above::Ref<Above::VertexBuffer> squareVB;
	squareVB.reset(Above::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout(
		{
		{Above::ShaderDataType::Float3, "a_Position"},
		});

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Above::Ref<Above::IndexBuffer> squareIB;
	squareIB.reset(Above::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Above::Shader::Create("assets/shaders/FlatColor.glsl");
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

	Above::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Above::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Above::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Above::Renderer::EndScene();
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
