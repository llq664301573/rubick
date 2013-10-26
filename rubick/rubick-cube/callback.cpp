#include "callback.h"

#include "Picker.h"
#include "utility.h"
#include "global.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		mouseButtonRightReleased = true;

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mouseButtonRightLastpos.x = xpos;
		mouseButtonRightLastpos.y = ypos;

		mouseButtonRightReleased = false;
	}

	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		picker.picking(xpos, ypos, width, height);

		if(action == GLFW_RELEASE)
		{
			if(xpos < 0 || xpos >= width || ypos < 0 || ypos > height)
				picker.face[0] = picker.face[1] = NULL;
			else
				picker.tryRotary();
		}
	}
}

void cursorpos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!mouseButtonRightReleased)// Û±Í”“º¸Õœ∂Ø
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		if(xpos < 0 || xpos >= width || ypos < 0 || ypos > height)
			return ;

		vec3 v1 = trackball_ptov(mouseButtonRightLastpos.x, mouseButtonRightLastpos.y, width, height);
		vec3 v2 = trackball_ptov(xpos, ypos, width, height);

		vec3 axis = cross(v1, v2);
		float angle = acos(dot(v1, v2)) * 180 / M_PI;

		camera = rotate(mat4(), angle, axis) * camera;//mat4_cast(angleAxis(angle, axis))”–bug

		mouseButtonRightLastpos.x = xpos;
		mouseButtonRightLastpos.y = ypos;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	static float times = 1.0f;
	times += yoffset * 0.1f;
	if(times < 0.1f)
		times = 0.1f;
	if(times > 10.0f)
		times = 10.0f;

	scroll = scale(mat4(), vec3(times, times, times));
}