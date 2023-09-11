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

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};