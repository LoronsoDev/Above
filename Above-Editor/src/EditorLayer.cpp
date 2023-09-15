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

		m_Square = m_ActiveScene->CreateEntity("Square");
		m_Square.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));

		m_Camera = m_ActiveScene->CreateEntity("Camera Entity");
		auto& cameraComponent = m_Camera.AddComponent<CameraComponent>(glm::ortho(-16.f, 16.0f, -9.f, 9.0f, -100.f, 100.f));
		cameraComponent.Primary = true;

		m_SecondCamera = m_ActiveScene->CreateEntity("Clipspace camera");
		auto& secondCameraComponent = m_SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.f, 1.0f, -1.f, 1.0f, -100.f, 100.f));
	}

	void EditorLayer::OnDetach()
	{
		AB_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		AB_PROFILE_FUNCTION();


		/*if(m_ViewportFocused)
			m_CameraController.OnUpdate(timestep);*/

		//render

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ .1f, .1f, .1f, 1.0f });
		RenderCommand::Clear();

		//Update scene
		m_ActiveScene->OnUpdate(timestep);

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
			if (m_Square)
			{
				auto& squareColor = m_Square.GetComponent<SpriteRendererComponent>().Color;
				ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
				ImGui::Separator();
			}

			ImGui::DragFloat3("Camera transform", glm::value_ptr(m_Camera.GetComponent<TransformComponent>().Transform[3]));

			static bool clipCamera = false;

			if (ImGui::Checkbox("Clip camera", &clipCamera))
			{
				m_SecondCamera.GetComponent<CameraComponent>().Primary = clipCamera;
				m_Camera.GetComponent<CameraComponent>().Primary = !clipCamera;
			}
			ImGui::Separator();

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