#pragma once

#include "common.h"
#include "Face.h"

class Cube
{
public:
	Cube(int8 x, int8 y, int8 z, int index);

	static void init(vector<Cube*> &cubes, const int xSize = 3, const int ySize = 3, const int zSize = 3);
	static void clear(vector<Cube*> &cubes);

	static vec3 size;
	int index;//´ýÒÆ³ý
	vec4 pos;
	mat4 pickMatirx;
	mat4 modelMatrix;
};
