#pragma once

#include "common.h"
#include "Face.h"

class Cube
{
public:
	Cube(int8 x, int8 y, int8 z);

	static bool init(vector<Cube*> &cubes);
	static void clear(vector<Cube*> &cubes);

	static vec3 size;
	vec4 pos;
	mat4 pickMatirx;
	mat4 modelMatrix;
};
