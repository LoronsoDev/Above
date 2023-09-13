#include <abpch.h>
#include "Framebuffer.h"

#include <Above/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLFramebuffer.h>

namespace Above
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferProperties& props)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFramebuffer>(props);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
