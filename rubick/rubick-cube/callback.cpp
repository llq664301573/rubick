#include "callback.h"

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

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		vec4 v;
		v.x = (2.0f*xpos - width) / width;
		v.y = (height - 2.0f*ypos) / height;  
		v.z = -1.0f;//[-1, 1] 由近及远
		v.w = 1;

		// Transform the screen space Pick ray into 3D space
		//v = inverse(projection * view) * v;
		v = inverse(projection) * v;
		v = inverse(view) * v;

		vec3 vPickRayOrig = vec3(v.x / v.w, v.y / v.w, v.z / v.w);//屏幕鼠标点世界空间坐标
		vec3 vPickRayDir = vPickRayOrig - eye;

		float distance = FLT_MAX;
		for (int i=0;i<cubes.size();i++)
		{
			static int cnt = 0;
			for (int j=0;j<12;j++)//每个立方体十二个三角形
			{
				const float *pbuffer = vertex_buffer_data + j * 9;

				vec3 vertex[3];
				for (int k=0;k<3;k++)//三角形上三点
				{
					vec4 v = vec4(pbuffer[k*3], pbuffer[k*3+1], pbuffer[k*3+2], 1);
					v = camera * scroll * cubes[i]->model * v;//变换到世界空间

					vertex[k] = vec3(v.x, v.y, v.z);
				}

				float t, u, v;
				if(IntersectTriangle(vPickRayOrig, vPickRayDir, vertex[0], vertex[1], vertex[2], &t, &u, &v) && t < distance)
				{
					pick.cube = i;
					pick.index = j * 3;

					distance = t;
				}
			}
		}
	}
}

void cursorpos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!mouseButtonRightReleased)//鼠标右键拖动
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		vec3 v1 = trackball_ptov(mouseButtonRightLastpos.x, mouseButtonRightLastpos.y, width, height);
		vec3 v2 = trackball_ptov(xpos, ypos, width, height);

		vec3 axis = cross(v1, v2);
		float angle = acos(dot(v1, v2)) * 180 / M_PI;

		camera = rotate(mat4(), angle, axis) * camera;//mat4_cast(angleAxis(angle, axis))有bug

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