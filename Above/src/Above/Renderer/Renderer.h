#pragma once

#include "RendererAPI.h"

namespace Above
{
	class Renderer
	{
	public:
		static void BeginScene(); //TODO
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

