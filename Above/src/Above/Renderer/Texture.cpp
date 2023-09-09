#include "abpch.h"
#include "Texture.h"

#include <Above/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Above
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
