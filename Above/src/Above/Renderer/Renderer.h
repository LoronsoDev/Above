#pragma once

namespace Above
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL,
		Direct3D,
		Vulkan,
		Count
	};

	class Renderer
	{
	public:
		static inline RendererAPI GetAPI() { return s_RendererAPI; };

	private:
		static RendererAPI s_RendererAPI;
	};
}

