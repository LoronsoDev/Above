#pragma once

#include <Above.h>

class Sandbox2D : public Above::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Above::Timestep timestep) override;
	void OnImGuiRender() override;
	void OnEvent(Above::Event& event) override;

private:
	Above::OrthographicCameraController m_CameraController;

	//temp
	Above::Ref<Above::VertexArray> m_SquareVA;
	Above::Ref<Above::Shader> m_FlatColorShader;
	Above::Ref<Above::Texture2D> m_CheckerboardTexture;
	Above::Ref<Above::Texture2D> m_SpriteSheet;

	Above::Ref<Above::SubTexture2D> m_StreetTopEdge_ST;
	Above::Ref<Above::SubTexture2D> m_Street_ST;
	Above::Ref<Above::SubTexture2D> m_StreetBottomEdge_ST;

	Above::Ref<Above::SubTexture2D> m_RoadTopEdge_ST;
	Above::Ref<Above::SubTexture2D> m_MainRoad_ST;
	Above::Ref<Above::SubTexture2D> m_RoadMiddle_ST;

	// The tilemap we're using has 1036 available tiles
	std::array<Above::Ref<Above::SubTexture2D>, 'Z'> m_MapElements{};

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};