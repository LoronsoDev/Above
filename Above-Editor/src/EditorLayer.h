#pragma once

#include <Above.h>

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
		OrthographicCameraController m_CameraController;

		//temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;

		Ref<SubTexture2D> m_StreetTopEdge_ST;
		Ref<SubTexture2D> m_Street_ST;
		Ref<SubTexture2D> m_StreetBottomEdge_ST;

		Ref<SubTexture2D> m_RoadTopEdge_ST;
		Ref<SubTexture2D> m_MainRoad_ST;
		Ref<SubTexture2D> m_RoadMiddle_ST;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;

		Entity m_Square;
		Entity m_Camera;
		Entity m_SecondCamera;
		char shader[1024] = 
R"(
#type vertex

#type fragment


)";

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = glm::vec2(0.f);

		// The tilemap we're using has 1036 available tiles
		std::array<Above::Ref<SubTexture2D>, 'Z'> m_MapElements{};

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

		float fps = 0.0f;
	};
}