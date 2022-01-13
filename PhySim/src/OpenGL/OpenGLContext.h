#pragma once

struct GLFWwindow;

namespace PhySim {

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();
	
	private:
		GLFWwindow* m_WindowHandle;
	};
}