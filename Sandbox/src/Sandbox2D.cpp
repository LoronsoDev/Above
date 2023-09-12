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
	AB_PROFILE_FUNCTION();

	m_CheckerboardTexture = Above::Texture2D::Create("assets/textures/checkerboard.png");
	m_SpriteSheet = Above::Texture2D::Create("assets/game/textures/tilemap_packed.png");

	m_SubTexture = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {31,2}, {16,16}, {3, 2});
}

void Sandbox2D::OnDetach()
{
	AB_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Above::Timestep timestep)
{
	AB_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(timestep);

	Above::Renderer2D::ResetStats();
	{
		AB_PROFILE_SCOPE("Renderer Prep");

		//render
		Above::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
		Above::RenderCommand::Clear();
	}

#if 0
	{
		AB_PROFILE_SCOPE("Render Draw");

		Above::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static float rotation = 0.0f;
		rotation += timestep * 25.f;

		//Above::Renderer2D::DrawRotatedQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, 45.f, { 0.8f, .2f, .3f, 1.f });
		Above::Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, .2f, .3f, 1.f });
		Above::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, glm::radians(rotation), { 0.2f, .3f, .8f, 1.f });
		Above::Renderer2D::DrawQuad({ 0.2f, -0.5f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 12.0f, { 0.2f, 0.15f, 0.2f, 1.0f });
		Above::Renderer2D::DrawRotatedQuad({ -0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 45.f, m_CheckerboardTexture, 1.0f);

		Above::Renderer2D::EndScene();

		Above::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (int y = -15; y < 15; y++)
		{
			for (int x = -15; x < 15; x++)
			{
				float r = (((float)x + 15.f) / 30.f);
				float g = (((float)y + 15.f) / 30.f);
				float b = 1.f;
				float a = 0.8f;

				glm::vec4 color(r, g, b, a);
				Above::Renderer2D::DrawRotatedQuad({ (float)-y * 0.125f, (float)-x * 0.125f, 0.2f }, { 0.25f, 0.25f }, rotation, m_CheckerboardTexture, 1.0f, color);
			}
		}

		Above::Renderer2D::EndScene();
		}
#endif

		Above::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static glm::vec2 position = glm::vec2(0);
		static float directionSign = 1;
		static float multiplier = 1;
		if(position.x < -1.f || position.x > 1.f)
		{
			directionSign *= -1;
		}
		multiplier = (1 - glm::abs(position.x)) + 0.25f;
		position += timestep * 0.5 * directionSign * multiplier;

		for(int i = -1; i < 1; ++i)
		{
			Above::Renderer2D::DrawQuad({ i * 1.25f + position.x, 0.0f, i * 0.002f}, {1.5f, 1.0f}, m_SubTexture, 1.0f);
		}
		//Above::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f);
		Above::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	AB_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	auto stats = Above::Renderer2D::GetStats();
	ImGui::Begin("Statistics");
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d:", stats.Drawcalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void Sandbox2D::OnEvent(Above::Event& e)
{
	m_CameraController.OnEvent(e);
}
