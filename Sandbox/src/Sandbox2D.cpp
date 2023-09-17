#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <Platform/OpenGL/OpenGLShader.h>

static const char* s_MapTiles =

"SSSSSSSSSSSSSSSSSSSSSSSS"
"DDDDDDDDDDDDDDDDDDDDDDDD"
"TTTTTTTTTTTTTTTTTTTTTTTT"
"RRRRRRRRRRRRRRRRRRRRRRRR"
"MMMMMMMMMMMMMMMMMMMMMMMM"
"RRRRRRRRRRRRRRRRRRRRRRRR"
"BBBBBBBBBBBBBBBBBBBBBBBB"
"UUUUUUUUUUUUUUUUUUUUUUUU"
"SSSSSSSSSSSSSSSSSSSSSSSS"
;

static const uint32_t s_MapWidth = 24;
static const uint32_t s_MapHeight = strlen(s_MapTiles)/s_MapWidth;

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

	m_StreetBottomEdge_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {26,26}, {16,16}, {1, 1});
	m_Street_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {30,27}, {16,16}, {1, 1});
	m_StreetTopEdge_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {26,27}, {16,16}, {1, 1});

	m_RoadTopEdge_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,7}, {16,16}, {1, 1});
	m_RoadMiddle_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {9,8}, {16,16}, {1, 1});
	m_MainRoad_ST = Above::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,8}, {16,16}, {1, 1});

	m_MapElements['T'] = m_RoadTopEdge_ST;
	m_MapElements['M'] = m_RoadMiddle_ST;
	m_MapElements['R'] = m_MainRoad_ST;
	m_MapElements['B'] = m_RoadTopEdge_ST;

	m_MapElements['U'] = m_StreetTopEdge_ST;
	m_MapElements['S'] = m_Street_ST;
	m_MapElements['D'] = m_StreetBottomEdge_ST;

	m_CameraController.SetZoomLevel(5.00f);
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
		Above::Renderer2D::BeginScene(m_CameraController.GetCamera());

		const int road_length = 5;

		for(int road_n = 0; road_n < road_length; ++road_n)
		{
			for (int i = 0; i < s_MapHeight * s_MapWidth; ++i)
			{
				glm::vec3 position = { ((s_MapWidth * road_n) - (i % s_MapWidth)), s_MapHeight - (i / s_MapWidth), -i * 0.001f };
				position -= glm::vec3{ (float)s_MapWidth / 2.f, ((float)s_MapHeight / 2.f),  0.f };

				float rotation = 0.0f;

				switch (s_MapTiles[i])
				{
				case 'B': //Road (bottom)
					rotation = glm::radians(180.f);
					break;
				}

				Above::Renderer2D::DrawRotatedQuad(position, { 1.0f, 1.0f }, rotation, m_MapElements[s_MapTiles[i]], 1.0f);
			}
		}
		

		/*Above::Renderer2D::DrawQuad({ position.x, position.y, 0.0f}, {1.0f, 1.0f}, m_Street_ST, 1.0f);
		Above::Renderer2D::DrawQuad({ position.x, position.y - 1.0f, 0.0f}, {1.0f, 1.0f}, m_StreetBottomEdge_ST, 1.0f);
		Above::Renderer2D::DrawQuad({ position.x, position.y - 2.0f, 0.0f}, {1.0f, 1.0f}, m_RoadTopEdge_ST, 1.0f);*/

		
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
	{
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw Calls: %d:", stats.Drawcalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	}
	ImGui::End();

}

void Sandbox2D::OnEvent(Above::Event& e)
{
	m_CameraController.OnEvent(e);
}
