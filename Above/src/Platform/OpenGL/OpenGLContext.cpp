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
		AB_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
		AB_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
		AB_CORE_INFO("    Version {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}