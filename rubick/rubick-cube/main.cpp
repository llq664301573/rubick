#include "shader.hpp"
#include "TextureManager/TextureManager.h"

#include <GL/glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32sd.lib")

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

typedef unsigned char byte;

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 480, "rubick-cube", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = loadShaders(2, "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	mat4 Projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	mat4 View = lookAt(vec3(0,0,10), vec3(0,0,0), vec3(0,1,0));
	mat4 Model = rotate(mat4(), 45.0f, vec3(1, 0, 0)) * rotate(mat4(), 45.0f, vec3(0, 1, 0));
	mat4 MVP = Projection * View * Model;


	const GLuint Texture = 1000;
	TextureManager::Inst()->LoadTexture("images/cube.tga", Texture, GL_BGRA);
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	const GLfloat vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,    //0
		-1.0f,-1.0f,1.0f,     //1
		-1.0f,1.0f,-1.0f,     //2
		-1.0f,-1.0f,1.0f,     //1
		-1.0f,1.0f,1.0f,      //3
		-1.0f,1.0f,-1.0f,     //2

		1.0f,-1.0f,-1.0f,     //4
		1.0f,1.0f,-1.0f,      //6
		1.0f,-1.0f,1.0f,      //5
		1.0f,-1.0f,1.0f,      //5
		1.0f,1.0f,-1.0f,      //6
		1.0f,1.0f,1.0f,       //7

		-1.0f,-1.0f,-1.0f,    //0
		1.0f,-1.0f,-1.0f,     //4
		-1.0f,-1.0f,1.0f,     //1
		-1.0f,-1.0f,1.0f,     //1
		1.0f,-1.0f,-1.0f,     //4
		1.0f,-1.0f,1.0f,      //5

		-1.0f,1.0f,-1.0f,     //2
		-1.0f,1.0f,1.0f,      //3
		1.0f,1.0f,-1.0f,      //6
		-1.0f,1.0f,1.0f,      //3
		1.0f,1.0f,1.0f,       //7
		1.0f,1.0f,-1.0f,      //6

		-1.0f,-1.0f,-1.0f,    //0
		-1.0f,1.0f,-1.0f,     //2
		1.0f,-1.0f,-1.0f,     //4
		-1.0f,1.0f,-1.0f,     //2
		1.0f,1.0f,-1.0f,      //6
		1.0f,-1.0f,-1.0f,     //4

		-1.0f,-1.0f,1.0f,     //1
		1.0f,-1.0f,1.0f,      //5
		-1.0f,1.0f,1.0f,      //3
		-1.0f,1.0f,1.0f,      //3
		1.0f,-1.0f,1.0f,      //5
		1.0f,1.0f,1.0f,       //7
	};

	const GLfloat uv_buffer_data[] = { 
		2.0f/3, 0.5f,
		3.0f/3, 0.5f,
		2.0f/3, 1.0f,
		3.0f/3, 0.5f,
		3.0f/3, 1.0f,
		2.0f/3, 1.0f,

		2.0f/3, 0.0f,
		3.0f/3, 0.0f,
		2.0f/3, 0.5f,
		2.0f/3, 0.5f,
		3.0f/3, 0.0f,
		3.0f/3, 0.5f,

		0.0f/3, 0.0f,
		1.0f/3, 0.0f,
		0.0f/3, 0.5f,
		0.0f/3, 0.5f,
		1.0f/3, 0.0f,
		1.0f/3, 0.5f,

		1.0f/3, 0.0f,
		2.0f/3, 0.0f,
		1.0f/3, 0.5f,
		2.0f/3, 0.0f,
		2.0f/3, 0.5f,
		1.0f/3, 0.5f,

		1.0f/3, 0.5f,
		2.0f/3, 0.5f,
		1.0f/3, 1.0f,
		2.0f/3, 0.5f,
		2.0f/3, 1.0f,
		1.0f/3, 1.0f,

		0.0f/3, 0.5f,
		1.0f/3, 0.5f,
		0.0f/3, 1.0f,
		0.0f/3, 1.0f,
		1.0f/3, 0.5f,
		1.0f/3, 1.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, (float*)&MVP);

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


		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}