#include <abpch.h>
#include <Above/Renderer/VertexArray.h>

#include <Above/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Above
{
	VertexArray* Above::VertexArray::Create()
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}