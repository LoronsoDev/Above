#include <abpch.h>

#include <Above/Renderer/OrthographicCameraController.h>

#include <Above/Core/Input.h>
#include <Above/Core/Keycodes.h>

namespace Above
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		AB_PROFILE_FUNCTION();

		m_CameraTranslationSpeed = m_ZoomLevel;

		if (Input::IsKeyPressed(AB_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(AB_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(AB_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(AB_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);

		if (!m_Rotation) return;
		
		if (Input::IsKeyPressed(AB_KEY_E))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		if (Input::IsKeyPressed(AB_KEY_Q))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}

		m_Camera.SetRotation(m_CameraRotation);

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		AB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateProjection()
	{
		m_ZoomLevel = std::max(m_ZoomLevel, 0.05f);
		m_ZoomLevel = std::min(m_ZoomLevel, 5.f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		AB_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.05f);
		m_ZoomLevel = std::min(m_ZoomLevel, 5.f);
		CalculateProjection();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AB_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateProjection();
		return false;
	}
}
