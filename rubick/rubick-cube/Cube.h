#pragma once

#include "common.h"
#include "Face.h"

class Cube
{
public:
	Cube(int8 x, int8 y, int8 z, int index);

	static bool init(vector<Cube*> &cubes);
	static void clear(vector<Cube*> &cubes);

	static vec3 size;
	int index;//´ýÒÆ³ý
	vec4 pos;
	mat4 pickMatirx;
	mat4 modelMatrix;
};
