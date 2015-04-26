#pragma once
#include <ostream>

struct Face
{
	Face(unsigned short vert1, unsigned short vert2, unsigned short vert3);
	unsigned short vert1;
	unsigned short vert2;
	unsigned short vert3;
};

std::ostream& operator<<(std::ostream &stream, const Face &vert);