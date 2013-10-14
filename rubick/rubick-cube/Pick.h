#ifndef PICK_H
#define PICK_H

typedef unsigned char byte;

class Pick
{
public:
	Pick():cube(-1)
	{

	}

	short cube;
	byte index;
};

#endif