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
		//Seems like Visual Studio is in some form using the Windows SDK to compile.
		//Not casting this throws problems in W11 but not in W10.
		AB_CORE_INFO("    Vendor: {0}", reinterpret_cast<const char*>	(glGetString(GL_VENDOR)));
		AB_CORE_INFO("    Renderer: {0}", reinterpret_cast<const char*>	(glGetString(GL_RENDERER)));
		AB_CORE_INFO("    Version {0}", reinterpret_cast<const char*>	(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}