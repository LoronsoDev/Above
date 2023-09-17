#include "abpch.h"
#include "Texture.h"

#include <Above/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Above
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const uint32_t& textureID)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(textureID);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
