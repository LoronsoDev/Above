#pragma once

#include <Above.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Above
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep timestep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		std::string LoadScene();
		void SaveScene(std::string path);
		std::string SaveSceneAs();

	private:
		OrthographicCameraController m_CameraController;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_PostprocessFramebuffer;

		Ref<Scene> m_ActiveScene;
		std::string savePath;

		Entity m_Square;
		Entity m_Camera;
		Entity m_SecondCamera;
		
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = glm::vec2(0.f);
		glm::vec2 m_ViewportBounds[2];

		Entity m_PlayerCarEntity;
		std::vector<Entity> m_EnemyCarEntities;

		// The tilemap we're using has 1036 available tiles
		std::array<Above::Ref<SubTexture2D>, 'Z'> m_MapElements{};

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		float fps = 0.0f;


		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}