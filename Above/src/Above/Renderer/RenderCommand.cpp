#include <abpch.h>
#include "RenderCommand.h"

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Above
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}