#pragma once

#include <Above/Renderer/OrthographicCamera.h>
#include <Above/Core/Timestep.h>

#include <Above/Events/ApplicationEvent.h>
#include <Above/Events/MouseEvent.h>

namespace Above
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateProjection(); }

	private:
		void CalculateProjection();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		bool m_Rotation = false;
		
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 1.f;
		float m_CameraRotationSpeed = 90.f;
	};
}