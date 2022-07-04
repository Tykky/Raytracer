#include "Input.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include "logging/Logging.h"

namespace Editor
{
    // Forward declaration of functions defined in Editor.cpp

    GLFWwindow* getCurrentWindowHandle();

	// Forward declaration of internal stuff

	int keyCodeToGLFW(KeyCode code);
	int mouseButtonToGLFW(MouseCode button);
	void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	// Callbacks have access to these static variables

	static float MOUSE_SCROLL = 0.0f;

	// User input API impl

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

	Vec2d getCursorPos()
	{
		Vec2d v;
		glfwGetCursorPos(getCurrentWindowHandle(), &v.x, &v.y);
		return v;
	}

	void setMouseScrollCallback()
	{
		glfwSetScrollCallback(getCurrentWindowHandle(), mouseScrollCallback);
	}

	// Internal implementation

	int keyCodeToGLFW(KeyCode code)
	{
		return static_cast<int>(code);
	}

	int mouseButtonToGLFW(MouseCode button)
	{
		return static_cast<int>(button);
	}
	
	void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (MOUSE_SCROLL + yoffset < 0.1f)
		{
			MOUSE_SCROLL = 0.0f;
		}
		else
		{
			MOUSE_SCROLL += yoffset;
		}
	}
}