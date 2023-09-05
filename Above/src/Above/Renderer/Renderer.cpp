#include "abpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Above
{
	void Renderer::BeginScene()
	{
		//Does verifications, like making sure the shader gets the right uniforms.
	} 

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
