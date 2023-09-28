#include "EditorLayer.h"

#include <random>

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <Platform/OpenGL/OpenGLShader.h>

#include <Above/Scene/SceneSerializer.h>

#include <Above/Utils/PlatformUtils.h>

#include "Systems.h"

namespace Above
{
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
	static const uint32_t s_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	EditorLayer::EditorLayer()
		:
		Layer("EditorLayer"),
		m_CameraController(1280.0f / 720.0f)
	{
	}

	void OnCollisionRegistered(Entity thisEnt, Entity otherEnt)
	{

	}

	void EditorLayer::OnAttach()
	{
		AB_PROFILE_FUNCTION();

		FramebufferProperties framebufferProps;
		framebufferProps.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		framebufferProps.Width = 1280;
		framebufferProps.Height = 720;
		m_Framebuffer = Framebuffer::Create(framebufferProps);

		m_CameraController.SetZoomLevel(5.00f);

		m_ActiveScene = CreateRef<Scene>();

		Ref<Texture2D> spriteSheet = Above::Texture2D::Create("assets/textures/tilemap_packed.png");
		auto st2d = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 26,26 }, { 16,16 }, { 1, 1 });

		spriteSheet->Bind();

		auto streetTop = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 26,27 }, { 16,16 }, { 1, 1 });
		auto streetMain = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 30,27 }, { 16,16 }, { 1, 1 });
		auto streetBottom = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 26,26 }, { 16,16 }, { 1, 1 });

		auto roadTop = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 11,7 }, { 16,16 }, { 1, 1 });
		auto roadMid = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 9,8 }, { 16,16 }, { 1, 1 });
		auto roadMain = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 11,8 }, { 16,16 }, { 1, 1 });

		auto tree = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 32,16}, { 16,16 }, { 1, 2 });
		auto lightPole = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 2,9}, { 16,16 }, { 1, 3 });


		m_MapElements['T'] = roadTop;
		m_MapElements['M'] = roadMid;
		m_MapElements['R'] = roadMain;
		m_MapElements['B'] = roadTop;

		m_MapElements['U'] = streetTop;
		m_MapElements['S'] = streetMain;
		m_MapElements['D'] = streetBottom;

		m_MapElements['.'] = tree;
		m_MapElements['L'] = lightPole;

		const int road_length = 20;

		for (int road_n = 0; road_n < road_length; ++road_n)
		{
			for (int i = 0; i < s_MapHeight * s_MapWidth; ++i)
			{
				const int x = ((s_MapWidth * road_n) - (i % s_MapWidth));
				const int y = s_MapHeight - (i / s_MapWidth);
				const int z = -i * 0.001f;
				glm::vec3 position = { x, y, z };
				position -= glm::vec3{ (float)s_MapWidth / 2.f, ((float)s_MapHeight / 1.8f),  0.f };

				float rotation = 0.0f;

				switch (s_MapTiles[i])
				{
				case 'B': //Road (bottom)
					rotation = glm::radians(180.f);
					break;
				}
				
				auto entity = m_ActiveScene->CreateEntity(std::string("Scenary") + std::to_string(i));
				entity.AddComponent<SpriteRendererComponent>(m_MapElements[s_MapTiles[i]]);
				entity.GetComponent<TransformComponent>().Translation = position;
				entity.GetComponent<TransformComponent>().Rotation = glm::vec3(0.f,0.f,rotation);
			}
		}

		//Done again because of painting order

		for (int road_n = 0; road_n < road_length; ++road_n)
		{
			for (int i = 0; i < s_MapHeight * s_MapWidth; ++i)
			{
				const int x = ((s_MapWidth * road_n) - (i % s_MapWidth));
				const int y = s_MapHeight - (i / s_MapWidth);
				const int z = -i * 0.001f;
				glm::vec3 position = { x, y, z };
				position -= glm::vec3{ (float)s_MapWidth / 2.f, ((float)s_MapHeight / 1.8f),  0.f };

				float rotation = 0.0f;

				if (s_MapTiles[i] == 'S')
				{
					if (i % 3 > 1)
					{
						auto tree = m_ActiveScene->CreateEntity(std::string("tree") + std::to_string(i));
						tree.AddComponent<SpriteRendererComponent>(m_MapElements['.']);
						tree.GetComponent<TransformComponent>().Translation = position;
						tree.GetComponent<TransformComponent>().Translation.y *= 1.5f;
						tree.GetComponent<TransformComponent>().Translation.z += 0.5f;
						tree.GetComponent<TransformComponent>().Rotation = glm::vec3(0.f, glm::radians(-45.f), glm::radians(-90.f));
					}

				}

				if (s_MapTiles[i] == 'D')
				{
					if (i % 4 > 2)
					{
						auto lightPole = m_ActiveScene->CreateEntity(std::string("lightpole") + std::to_string(i));
						lightPole.AddComponent<SpriteRendererComponent>(m_MapElements['L']);
						lightPole.GetComponent<TransformComponent>().Translation = position;
						lightPole.GetComponent<TransformComponent>().Scale.y = 1.5f;
						lightPole.GetComponent<TransformComponent>().Translation.z += 0.5f;
						lightPole.GetComponent<TransformComponent>().Rotation = glm::vec3(0.f, glm::radians(-45.f), glm::radians(-90.f));
					}

				}

				if (s_MapTiles[i] == 'U')
				{
					if (i % 4 > 2)
					{
						auto lightPole = m_ActiveScene->CreateEntity(std::string("lightpole") + std::to_string(i));
						lightPole.AddComponent<SpriteRendererComponent>(m_MapElements['L']);
						lightPole.GetComponent<TransformComponent>().Translation = position;
						lightPole.GetComponent<TransformComponent>().Scale.y = 1.5f;
						lightPole.GetComponent<TransformComponent>().Translation.z += 0.5f;
						lightPole.GetComponent<TransformComponent>().Rotation = glm::vec3(0.f, glm::radians(-45.f), glm::radians(-90.f));
						lightPole.GetComponent<TransformComponent>().Scale.x = -1;
					}

				}
			}
		}

		auto mapLimit = s_MapWidth + (s_MapWidth * 0.5f);

		auto playerCar = Above::SubTexture2D::CreateFromCoords(spriteSheet, { 31,0 }, { 16,16 }, { 2, 2 });

		m_PlayerCarEntity = m_ActiveScene->CreateEntity("Player car");
		m_PlayerCarEntity.AddComponent<SpriteRendererComponent>(playerCar);
		m_PlayerCarEntity.GetComponent<TransformComponent>().Scale.y = 1.3f;
		glm::vec3 carPos = glm::vec3(-mapLimit + 3.f, 0.f, 0.05f);
		m_PlayerCarEntity.GetComponent<TransformComponent>().Translation = carPos;
		m_PlayerCarEntity.GetComponent<TransformComponent>().Rotation.z = glm::radians(-90.f);

		auto enemyCar = SubTexture2D::CreateFromCoords(spriteSheet, { 31,4 }, { 16,16 }, { 2, 2 });

		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator
		std::uniform_int_distribution<> distr(-5, 5); // define the range


		glm::vec3 nextEnemyCarPos = glm::vec3(- mapLimit + 20.f, -1.f, 0.4f);
		const int n_enemy_cars = 145;
		m_EnemyCarEntities.reserve(n_enemy_cars);
		for(int i = 0; i < n_enemy_cars; ++i)
		{
			auto enemyCarEntity = m_ActiveScene->CreateEntity(std::string("Enemy car ") + std::to_string(i));
			enemyCarEntity.AddComponent<SpriteRendererComponent>(enemyCar);
			enemyCarEntity.GetComponent<TransformComponent>().Scale.y = 1.3f;
			enemyCarEntity.GetComponent<TransformComponent>().Translation = nextEnemyCarPos;
			enemyCarEntity.GetComponent<TransformComponent>().Rotation.z = glm::radians(90.f);
			enemyCarEntity.AddComponent<NativeScriptComponent>().Bind<EnemyCarController>();
			
			nextEnemyCarPos.x = glm::clamp(nextEnemyCarPos.x, nextEnemyCarPos.x + 3.f, nextEnemyCarPos.x + 13.f + distr(gen));
			nextEnemyCarPos.y = (distr(gen) % 2 ? -1.f : 1.f) + ((float)distr(gen) * 0.1f);
			m_EnemyCarEntities.push_back(enemyCarEntity);
		}
		
		m_Camera = m_ActiveScene->CreateEntity("Camera A");
		auto& cameraComponent = m_Camera.AddComponent<CameraComponent>();
		cameraComponent.Primary = true;
		cameraComponent.Camera.SetPerspective(45.f, 0.010f, 1000.f);
		auto& camTransform = m_Camera.GetComponent<TransformComponent>();
		camTransform.Translation = glm::vec3(-mapLimit, 0.f, 8.f);
		camTransform.Rotation = glm::vec3(0.f, glm::radians(-37.5f), glm::radians(-90.f));

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& secondCameraComponent = m_SecondCamera.AddComponent<CameraComponent>();

		

		m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_PlayerCarEntity.AddComponent<NativeScriptComponent>().Bind<CarController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		AB_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		AB_PROFILE_FUNCTION();

		if (FramebufferProperties fbProps = m_Framebuffer->GetProperties();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(fbProps.Width != m_ViewportSize.x || fbProps.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.ResizeBounds(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		/*if(m_ViewportFocused)
			m_CameraController.OnUpdate(timestep);*/

			//render

		m_Framebuffer->Bind();
		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ .3f, .6f, .3f, 1.0f });
		RenderCommand::Clear();

		//Update scene
		m_ActiveScene->OnUpdate(timestep);

		static bool playerAlive = true;
		static int carsHit[3] = {-1, -1, -1};
		static int carsHitIndex = 0;
		auto& myTc = m_PlayerCarEntity.GetComponent<TransformComponent>();
		if (carsHitIndex == 3) playerAlive = false;

		if(playerAlive)
		{
			for (int i = 0; i < m_EnemyCarEntities.size(); ++i)
			{
				auto& tc = m_EnemyCarEntities[i].GetComponent<TransformComponent>();
				auto distance = glm::distance(myTc.Translation, tc.Translation);
				if (distance < 0.75f)
				{
					bool exec = true;
					for (int j = 0; j < 3; j++)
					{
						if (carsHit[j] == i)
						{
							exec = false;
						}
					}
					if (!exec) continue;

					auto& color = m_PlayerCarEntity.GetComponent<SpriteRendererComponent>().Color;
					m_EnemyCarEntities[i].RemoveComponent<SpriteRendererComponent>();
					color.g -= 0.3f;
					color.b -= 0.3f;
					AB_ERROR("HIT {0}, distance:{1}", m_EnemyCarEntities[i].GetComponent<TagComponent>().Tag, distance);
					carsHit[carsHitIndex++] = i;
				}
			}
		}

		if(!playerAlive)
		{
			auto& color = m_PlayerCarEntity.GetComponent<SpriteRendererComponent>().Color;
			color.r = 0.0f;
			color.g = 0.0f;
			color.b = 0.0f;

			m_PlayerCarEntity.RemoveComponent<NativeScriptComponent>();
			CameraController* cc = (CameraController*) m_Camera.GetComponent<NativeScriptComponent>().Instance;
			cc->speed = glm::clamp(cc->speed, 0.0f, glm::abs(cc->speed - (timestep * 2.f)));
		}

		//auto [mx, my] = ImGui::GetMousePos();
		//mx -= m_ViewportBounds[0].x;
		//my -= m_ViewportBounds[0].y;
		//glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportSize[0];
		//my = viewportSize.y - my;
		//int mouseX = (int)mx;
		//int mouseY = (int)my;

		//if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		//{
		//	int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
		//	//AB_CORE_WARN("Pixels = {0}", pixelData);
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

		//The framebuffer that is gonna get rendered into the ImGui window
		uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID(0);
		static bool visualizeDepth = false;

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
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);

		{
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			ImVec2 minWinSize = style.WindowMinSize;
			style.WindowMinSize.x = 350.0f;

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize = minWinSize;

			static bool showShaderEdit = false;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("FILE"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (!savePath.empty())
					{
						if (ImGui::MenuItem("Save", "Ctrl+S"))
						{
							SaveScene(savePath);
						}
					}
					if (ImGui::MenuItem("Save as...", "Ctrl+shift+S"))
					{
						SaveSceneAs();
					}
					if (ImGui::MenuItem("Load...", "Ctrl+O"))
					{
						LoadScene();
					}

					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					ImGui::MenuItem("Quit") ? Application::Get().Close() : false;
					//
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("PROJECT"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.

					if (ImGui::MenuItem("Postprocess layer"))
					{
						showShaderEdit = true;
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("VIEW"))
				{
					ImGui::Checkbox("Visualize Depth", &visualizeDepth);

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();

				/*if (showShaderEdit)
				{
					ImGui::Begin("Postprocess shader");
					ImGui::Columns(2);
					if (ImGui::Button("Apply"))
					{
					}
					ImGui::NextColumn();
					if (ImGui::Button("x"))
					{
						showShaderEdit = false;
					}
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::InputTextMultiline("GLSL postprocess input field", shader, IM_ARRAYSIZE(shader), ImVec2(700, 300));
					ImGui::Separator();
					ImGui::End();
				}*/
			}

			m_SceneHierarchyPanel.OnImGuiRender();

			auto stats = Renderer2D::GetStats();
			ImGui::Begin("Statistics");
			{
				ImGui::Text("Renderer2D stats:");
				ImGui::Separator();
				ImGui::Text("FPS: %d (%.2fms frame time)", (uint32_t)fps, (1.f / fps) * 1000);
				ImGui::Text("Draw Calls: %d", stats.Drawcalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });

			ImGui::Begin("Viewport");
			auto viewportOffset = ImGui::GetCursorPos();

			auto windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = ImGui::GetWindowPos();
			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			m_ViewportBounds[0] = { minBound.x, minBound.y };
			m_ViewportBounds[1] = { maxBound.x, maxBound.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportSize.x, viewportSize.y };

			if (visualizeDepth)
			{
				texID = m_Framebuffer->GetDepthAttachmentRendererID(0);
			}
			ImGui::Image((void*)texID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();

			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if(e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool ctrl = Input::IsKeyPressed(AB_KEY_RIGHT_CONTROL) || Input::IsKeyPressed(AB_KEY_LEFT_CONTROL);
		bool shift = Input::IsKeyPressed(AB_KEY_LEFT_SHIFT) || Input::IsKeyPressed(AB_KEY_RIGHT_SHIFT);
		switch(e.GetKeyCode())
		{
		case AB_KEY_N:
			if (ctrl)
			{
				NewScene();
			}
			break;

		case AB_KEY_S:
			if(ctrl && shift)
			{
				SaveSceneAs();
			}
			else
			{
				if(!savePath.empty())
					SaveScene(savePath);
			}
			break;

		case AB_KEY_O:
			if (ctrl)
			{
				LoadScene();
			}
			break;
		}
		
	
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		savePath.clear();
	}

	std::string EditorLayer::LoadScene()
	{
		savePath = FileDialogs::OpenFile("ABOVE scene file (*.ab)\0*.ab\0*.above\0");
		if (!savePath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(savePath);
		}
		return savePath;
	}

	void EditorLayer::SaveScene(std::string path)
	{
		SceneSerializer serializer(m_ActiveScene);
		serializer.SerializeText(path);
	}

	std::string EditorLayer::SaveSceneAs()
	{
		savePath = FileDialogs::SaveFile("ABOVE scene file (*.ab)\0*.ab\0*.above\0");
		if (!savePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeText(savePath);
		}

		return savePath;
	}
}
