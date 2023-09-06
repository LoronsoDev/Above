#include "abpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Above
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//Does verifications, like making sure the shader gets the right uniforms.
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	} 

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
