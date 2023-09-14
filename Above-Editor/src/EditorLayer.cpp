#include "EditorLayer.h"

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

namespace Above
{
	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		AB_PROFILE_FUNCTION();

		FramebufferProperties framebufferProps;
		framebufferProps.Width = 1280;
		framebufferProps.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferProps);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");
		m_SpriteSheet = Texture2D::Create("assets/game/textures/tilemap_packed.png");

		m_StreetBottomEdge_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 26,26 }, { 16,16 }, { 1, 1 });
		m_Street_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 30,27 }, { 16,16 }, { 1, 1 });
		m_StreetTopEdge_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 26,27 }, { 16,16 }, { 1, 1 });

		m_RoadTopEdge_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,7 }, { 16,16 }, { 1, 1 });
		m_RoadMiddle_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 9,8 }, { 16,16 }, { 1, 1 });
		m_MainRoad_ST = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,8 }, { 16,16 }, { 1, 1 });

		m_MapElements['T'] = m_RoadTopEdge_ST;
		m_MapElements['M'] = m_RoadMiddle_ST;
		m_MapElements['R'] = m_MainRoad_ST;
		m_MapElements['B'] = m_RoadTopEdge_ST;

		m_MapElements['U'] = m_StreetTopEdge_ST;
		m_MapElements['S'] = m_Street_ST;
		m_MapElements['D'] = m_StreetBottomEdge_ST;

		m_CameraController.SetZoomLevel(5.00f);
	}

	void EditorLayer::OnDetach()
	{
		AB_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		AB_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(timestep);

		Renderer2D::ResetStats();
		{
			AB_PROFILE_SCOPE("Renderer Prep");

			//render
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
			RenderCommand::Clear();
		}

#if 0
		{
			AB_PROFILE_SCOPE("Render Draw");

			Renderer2D::BeginScene(m_CameraController.GetCamera());

			static float rotation = 0.0f;
			rotation += timestep * 25.f;

			//Renderer2D::DrawRotatedQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, 45.f, { 0.8f, .2f, .3f, 1.f });
			Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, .2f, .3f, 1.f });
			Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, glm::radians(rotation), { 0.2f, .3f, .8f, 1.f });
			Renderer2D::DrawQuad({ 0.2f, -0.5f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 12.0f, { 0.2f, 0.15f, 0.2f, 1.0f });
			Renderer2D::DrawRotatedQuad({ -0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 45.f, m_CheckerboardTexture, 1.0f);

			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (int y = -15; y < 15; y++)
			{
				for (int x = -15; x < 15; x++)
				{
					float r = (((float)x + 15.f) / 30.f);
					float g = (((float)y + 15.f) / 30.f);
					float b = 1.f;
					float a = 0.8f;

					glm::vec4 color(r, g, b, a);
					Renderer2D::DrawRotatedQuad({ (float)-y * 0.125f, (float)-x * 0.125f, 0.2f }, { 0.25f, 0.25f }, rotation, m_CheckerboardTexture, 1.0f, color);
				}
			}

			Renderer2D::EndScene();
		}
#endif

		Renderer2D::BeginScene(m_CameraController.GetCamera());

		const int road_length = 5;

		for (int road_n = 0; road_n < road_length; ++road_n)
		{
			for (int i = 0; i < s_MapHeight * s_MapWidth; ++i)
			{
				glm::vec3 position = { ((s_MapWidth * road_n) - (i % s_MapWidth)), s_MapHeight - (i / s_MapWidth), -i * 0.001f };
				position -= glm::vec3{ (float)s_MapWidth / 2.f, ((float)s_MapHeight / 2.f), 0.f };

				float rotation = 0.0f;

				switch (s_MapTiles[i])
				{
				case 'B': //Road (bottom)
					rotation = glm::radians(180.f);
					break;
				}

				Renderer2D::DrawRotatedQuad(position, { 1.0f, 1.0f }, rotation, m_MapElements[s_MapTiles[i]], 1.0f);
			}
		}


		/*Renderer2D::DrawQuad({ position.x, position.y, 0.0f}, {1.0f, 1.0f}, m_Street_ST, 1.0f);
		Renderer2D::DrawQuad({ position.x, position.y - 1.0f, 0.0f}, {1.0f, 1.0f}, m_StreetBottomEdge_ST, 1.0f);
		Renderer2D::DrawQuad({ position.x, position.y - 2.0f, 0.0f}, {1.0f, 1.0f}, m_RoadTopEdge_ST, 1.0f);*/


		//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f);
		Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		AB_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		{
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("FILE"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Quit") ? Application::Get().Close() : false;
					ImGui::EndMenu();
				}


				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
			ImGui::End();

			auto stats = Renderer2D::GetStats();
			ImGui::Begin("Statistics");
			{
				ImGui::Text("Renderer2D stats:");
				ImGui::Text("Draw Calls: %d:", stats.Drawcalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});

			ImGui::Begin("Viewport");
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportSize))
			{
				m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				m_ViewportSize = { viewportSize.x, viewportSize.y };

				m_CameraController.ResizeBounds(viewportSize.x, viewportSize.y);
			}

			uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)texID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::End();

			ImGui::PopStyleVar();

		}

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}