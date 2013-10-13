#include "Cube.h"


Cube::Cube(int8 x, int8 y, int8 z)
{
	model = translate(mat4(), vec3(x, y, z));
}

void Cube::init(vector<Cube*> &cubes, const int xSize, const int ySize, const int zSize)
{
	const int xMax = xSize / 2 * 2 - (xSize % 2 == 0);
	const int yMax = ySize / 2 * 2 - (ySize % 2 == 0);
	const int zMax = zSize / 2 * 2 - (zSize % 2 == 0);

	for (int z = -zMax; z <= zMax; z+=2)
	{
		for (int y = -yMax; y <= yMax; y+=2)
		{
			for (int x = -xMax; x <= xMax; x+=2)
			{
				cubes.push_back(new Cube(x,y,z));
			}
		}
	}
}