#pragma once

#include "RendererAPI.h"
#include <Above/Renderer/OrthographicCamera.h>
#include <Above/Renderer/Shader.h>

namespace Above
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}

