#pragma once

#include "global.h"
class Face;

struct Rotation
{
	vec3 axis;
	vec3 pivot;
};

class Picker
{
public:
	Picker();
	
	void picking(double xpos, double ypos, int width, int height);
	bool tryRotary();

public:
	Rotation rotation;
	
//private:

	float angle;
	Face* face[2];
};
