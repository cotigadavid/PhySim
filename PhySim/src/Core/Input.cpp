#include "pspch.h"
#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>

namespace PhySim {

	Input* Input::s_Instance = new Input();

	inline bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	inline bool Input::IsMouseButtonPressed(int mousebuttoncode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, mousebuttoncode);
		return state == GLFW_PRESS;
	}

	inline std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	inline float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	inline float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}