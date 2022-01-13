#include "pspch.h"
#include "OpenGLContext.h"

#include "Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace PhySim {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PS_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PS_ASSERT(status, "Failed to intialize Glad");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}