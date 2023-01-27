#include "abpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Above
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_WindowHandle(windowHandle)
	{
		AB_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AB_CORE_ASSERT(status, "FAILED TO INITIALIZE GLAD");

		AB_CORE_INFO("- OpenGL info -");

		//@Loren
		//I've been doing a bit of research on this as only some computers need a cast to char*
		// AFAIK:
		//Depending on the order of the linking proccess/includes, glGetString can conflict with the logger
		//as it won't know if GLubyte == const unsigned char*. It will try a static_cast and it will
		//retrieve an error, as it's not doable as far as it knows at that point.
		AB_CORE_INFO("    Vendor: {0}", reinterpret_cast<const char*>	(glGetString(GL_VENDOR)));
		AB_CORE_INFO("    Renderer: {0}", reinterpret_cast<const char*>	(glGetString(GL_RENDERER)));
		AB_CORE_INFO("    Version {0}", reinterpret_cast<const char*>	(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}