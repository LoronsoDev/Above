#include <abpch.h>
#include <Above/Renderer/Shader.h>

#include <Above/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace Above
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}