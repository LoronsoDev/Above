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

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");

		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, .8f, 1.f, 1.f));
	}

	void EditorLayer::OnDetach()
	{
		AB_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		AB_PROFILE_FUNCTION();

		Renderer2D::ResetStats();

		if(m_ViewportFocused)
			m_CameraController.OnUpdate(timestep);

		//render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
		RenderCommand::Clear();
		

		Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Update scene
		m_ActiveScene->OnUpdate(timestep);

		Renderer2D::EndScene();

		////AB_PROFILE_SCOPE("Render Draw"); //atm has a bug in some compilers (for example in W11 compiler), it's not concatenating macro ##__LINE__
		//{
		//	Renderer2D::BeginScene(m_CameraController.GetCamera());

		//	const int road_length = 5;

		//	for (int road_n = 0; road_n < road_length; ++road_n)
		//	{
		//		for (int i = 0; i < s_MapHeight * s_MapWidth; ++i)
		//		{
		//			glm::vec3 position = { ((s_MapWidth * road_n) - (i % s_MapWidth)), s_MapHeight - (i / s_MapWidth), -i * 0.001f };
		//			position -= glm::vec3{ (float)s_MapWidth / 2.f, ((float)s_MapHeight / 2.f), 0.f };

		//			float rotation = 0.0f;

		//			switch (s_MapTiles[i])
		//			{
		//			case 'B': //Road (bottom)
		//				rotation = glm::radians(180.f);
		//				break;
		//			}

		//			Renderer2D::DrawRotatedQuad(position, { 1.0f, 1.0f }, rotation, m_MapElements[s_MapTiles[i]], 1.0f);
		//		}
		//	}

		//	Renderer2D::EndScene();
		//}

		fps = 1.f / timestep;

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
			if (m_SquareEntity)
			{
				auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
				ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
				ImGui::Separator();
			}
			ImGui::End();
			
			auto stats = Renderer2D::GetStats();
			ImGui::Begin("Statistics");
			{
				ImGui::Text("Renderer2D stats:");
				ImGui::Text("FPS: %d", (uint32_t)fps);
				ImGui::Text("Draw Calls: %d", stats.Drawcalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});

			ImGui::Begin("Viewport");

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

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