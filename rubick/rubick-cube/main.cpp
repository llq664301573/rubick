#include "global.h"
#include "callback.h"
#include "constant.h"
#include "Cube.h"
#include "Picker.h"
#include "shader.h"
#include "TextureManager/TextureManager.h"

#pragma comment(lib, "opengl32.lib")

#ifdef _DEBUG
#pragma comment(lib, "glew32sd.lib")
#pragma comment(lib, "glfw3d.lib")
#else
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#endif // _DEBUG


int main()
{
	Cube::init(cubes);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 480, "rubick-cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSetCursorPosCallback(window, cursorpos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return EXIT_FAILURE;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = loadShaders(2, "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	projection = perspective(45.0f, 4.0f / 3.0f, 1.0f, 1000.0f);
	view = lookAt(eye, vec3(0, 0, 0), vec3(0, 1, 0));

	camera = rotate(mat4(), 45.0f, vec3(1, 0, 0)) * rotate(mat4(), 45.0f, vec3(0, 1, 0));

	const GLuint Texture = 1000;
	TextureManager::Inst()->LoadTexture("images/cube.png", Texture, GL_BGRA);
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(TextureID);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                  // match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		picker.angle -= 15;
		for (int i=0;i<cubes.size();i++)
		{
			Cube* cube = cubes[i];

			mat4 MVP;
			if(picker.face[0] != NULL && picker.face[1] != NULL)
			{
				const vec3& pivot = picker.rotation.pivot;
				const vec3& axis = picker.rotation.axis;
				mat4 rotMatrix = rotate(mat4(), 15.0f, axis);
				const vec3& v1 = picker.face[0]->center;
				const vec3& v2 = picker.face[1]->center;
				if(pivot.x == v1.x && pivot.x == v2.x)
				{
					if(cube->pos.x == pivot.x)
					{
						cube->pickMatirx = rotMatrix * cube->pickMatirx;
						cube->pos = rotMatrix * cube->pos;

						if(picker.angle == 0)
							cube->pos = round(cube->pos);
					}
				}
				else if(pivot.y == v1.y && pivot.y == v2.y)
				{
					if(cube->pos.y == pivot.y)
					{
						cube->pickMatirx = rotMatrix * cube->pickMatirx;
						cube->pos = rotMatrix * cube->pos;

						if(picker.angle == 0)
							cube->pos = round(cube->pos);
					}
				}
				else
				{
					if(cube->pos.z == pivot.z)
					{
						cube->pickMatirx = rotMatrix * cube->pickMatirx;
						cube->pos = rotMatrix * cube->pos;

						if(picker.angle == 0)
							cube->pos = round(cube->pos);
					}
				}	
			}

			MVP = projection * view * camera * scroll * cube->pickMatirx * cube->modelMatrix;
			
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, (float*)&MVP);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		}

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		if(picker.angle == 0)
			picker.face[0] = picker.face[1] = NULL;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	Cube::clear(cubes);

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return EXIT_SUCCESS;
}