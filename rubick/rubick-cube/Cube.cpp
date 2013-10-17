#include "Cube.h"
#include "global.h"

vec3 Cube::size;

Cube::Cube(int8 x, int8 y, int8 z, int index)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	pos.w = 1;
	this->index = index;
	modelMatrix = translate(mat4(), vec3(x, y, z));
}

void Cube::init(vector<Cube*> &cubes, const int xSize, const int ySize, const int zSize)
{
	size = vec3(xSize, ySize, zSize);

	const int xMax = xSize / 2 * 2 - (xSize % 2 == 0);
	const int yMax = ySize / 2 * 2 - (ySize % 2 == 0);
	const int zMax = zSize / 2 * 2 - (zSize % 2 == 0);

	int cnt = 0;
	for (int z = -zMax; z <= zMax; z+=2)
	{
		for (int y = -yMax; y <= yMax; y+=2)
		{
			for (int x = -xMax; x <= xMax; x+=2)
			{
				Cube *cube = new Cube(x, y, z, cnt++);
				bool visible[]={x == -xMax, x == xMax, y == -yMax, y == yMax, z == -zMax, z == zMax};
				for (int i=0;i<6;i++)
				{
					if(visible[i])
						faces.push_back(new Face(cube, i));
				}

				cubes.push_back(cube);
			}
		}
	}
}

void Cube::clear(vector<Cube*> &cubes)
{
	for (int i=0;i<cubes.size();i++)
	{
		delete cubes[i];
		cubes[i] = NULL;
	}

	cubes.clear();
}