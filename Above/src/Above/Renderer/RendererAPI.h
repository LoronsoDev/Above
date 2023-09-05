#pragma once

#include <glm/glm.hpp>
#include <Above/Renderer/VertexArray.h>

namespace Above
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		//what API is active at the moment
		static API s_API;
	};
}