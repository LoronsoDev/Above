#include <abpch.h>

#include <Above/Scene/SceneCamera.h>

#include "glm/gtc/matrix_transform.hpp"

namespace Above
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveVerticalFov = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / height;

		if(!(float)width / height - m_AspectRatio < 0.05f)
			RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if(m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveVerticalFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			return;
		}

		//Ortho
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}

