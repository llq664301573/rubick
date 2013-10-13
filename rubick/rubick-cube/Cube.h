#ifndef CUBE_H
#define CUBE_H

#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace glm::detail;

class Cube
{
public:
	Cube(int8 x, int8 y, int8 z);

	static void init(vector<Cube*> &cubes, const int xSize = 3, const int ySize = 3, const int zSize = 3);
	static void clear(vector<Cube*> &cubes);

	mat4 model;
};

#endif