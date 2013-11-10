#include "Cube.h"
#include "global.h"

#include <libconfig.h++>
using namespace libconfig;
#pragma comment(lib, "libconfig++.lib")

vec3 Cube::size;

Cube::Cube(int8 x, int8 y, int8 z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	pos.w = 1;
	modelMatrix = translate(mat4(), vec3(x, y, z));
}

bool Cube::init(vector<Cube*> &cubes)
{
	int xSize = 3, ySize = 3, zSize = 3;

	Config cfg;
	// Read the file. If there is an error, report it and exit.
	try
	{
		cfg.readFile("config.cfg");
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file." << std::endl;
		return EXIT_FAILURE;
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return EXIT_FAILURE;
	}

	const Setting& root = cfg.getRoot();

	// Output a list of all books in the inventory.
	try
	{
		const Setting &window = root["dimension"];
		int count = window.getLength();

		window.lookupValue("x", xSize);
		window.lookupValue("y", ySize);
		window.lookupValue("z", zSize);
	}
	catch(const SettingNotFoundException &nfex)
	{
		// Ignore.
	}


	size = vec3(xSize, ySize, zSize);

	const int xMax = xSize / 2 * 2 - (xSize % 2 == 0);
	const int yMax = ySize / 2 * 2 - (ySize % 2 == 0);
	const int zMax = zSize / 2 * 2 - (zSize % 2 == 0);

	for (int z = -zMax; z <= zMax; z+=2)
	{
		for (int y = -yMax; y <= yMax; y+=2)
		{
			for (int x = -xMax; x <= xMax; x+=2)
			{
				Cube *cube = new Cube(x, y, z);
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