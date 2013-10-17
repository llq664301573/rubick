#include "Face.h"
#include "Cube.h"

Face::Face(Cube* cube, byte index):cube(cube),index(index)
{
	center = vec3(cube->pos);

	switch(index)
	{
	case 0:
	case 1:
		center.x += index & 1 ? 1 : -1;
		break;
	case 2:
	case 3:
		center.y += index & 1 ? 1 : -1;
		break;
	case 4:
	case 5:
		center.z += index & 1 ? 1 : -1;
		break;
	default:
		break;
	}
}