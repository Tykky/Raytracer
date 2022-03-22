#include "Input.h"
#include <GLFW/glfw3.h>
#include <cassert>

namespace Editor
{
    //---------------------------------------------------------//
    // Forward declaration of functions defined in Editor.cpp  //
    //---------------------------------------------------------//

    GLFWwindow* getCurrentWindowHandle();

	//---------------------------------------//
	// Forward declaration of internal stuff //
	//---------------------------------------//

	int keyCodeToGLFW(KeyCode code);
	int mouseButtonToGLFW(MouseCode button);
	void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseScrollCallback(GLFWwindow* window, double xoffset, double ypos);

	//-------------------------------------------------//
	// Callbacks have access to these static variables //
	//-------------------------------------------------//

	static float MOUSE_SCROLL = 0.0f;

	//---------------------//
	// User input API impl //
	//---------------------//

	StatusCode getKey(KeyCode code)
	{
		int status = glfwGetKey(getCurrentWindowHandle(), keyCodeToGLFW(code));
		assert(status == 0 || status == 1);
		return static_cast<StatusCode>(status);
	}

	StatusCode getMouseButton(MouseCode button)
	{
		int status = glfwGetMouseButton(getCurrentWindowHandle(), mouseButtonToGLFW(button));
		assert(status == 0 || status == 1);
		return static_cast<StatusCode>(status);
	}

	float getMouseScroll()
	{
		return MOUSE_SCROLL;
	}

	Vec2D<double> getCursorPos()
	{
		Vec2D<double> v;
		glfwGetCursorPos(getCurrentWindowHandle(), &v.x, &v.y);
		return v;
	}

	//---------------//
	// Internal impl //
	//---------------//

	int keyCodeToGLFW(KeyCode code)
	{
		return static_cast<int>(code);
	}

	int mouseButtonToGLFW(MouseCode button)
	{
		return static_cast<int>(button);
	}

	void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
	}

	void mouseScrollCallback(GLFWwindow* window, double xoffset, double ypos)
	{
	}
}