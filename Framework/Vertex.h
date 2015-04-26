#pragma once
#include <ostream>


struct Vertex
{
public:
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z, float w);

	float x;
	float y;
	float z;
	float w;
};

std::ostream& operator<<(std::ostream &stream, const Vertex &vert);