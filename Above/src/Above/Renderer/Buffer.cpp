#include "abpch.h"

#include "Above/Renderer/Buffer.h"
#include "Above/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Above
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref <VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		//We decide which rendering API we're using...
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		AB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		AB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}