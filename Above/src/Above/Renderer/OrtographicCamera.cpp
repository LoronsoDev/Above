#include <abpch.h>
#include <Above/Renderer/OrthographicCamera.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Above
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:
	m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -100.0f, 100.0f)),
	m_ViewMatrix(1.0f)
	{
		AB_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		AB_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		AB_PROFILE_FUNCTION();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}