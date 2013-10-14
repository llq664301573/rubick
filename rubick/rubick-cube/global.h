#ifndef GLOBAL_H
#define GLOBAL_H

#include "shader.hpp"
#include "TextureManager/TextureManager.h"
#include "Cube.h"
#include "Pick.h"
#include "utility.h"
#include "constant.h"
#include "callback.h"

#include <GL/glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32sd.lib")

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

extern mat4 camera;//右键旋转变换
extern mat4 projection;//投影变换
extern mat4 view;//视图变换
extern mat4 scroll;//滚轮缩放变换

extern ivec2 mouseButtonRightLastpos;
extern bool mouseButtonRightReleased;

extern Pick pick;

extern vector<Cube*> cubes;

#endif