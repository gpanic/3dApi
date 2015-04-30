#pragma once
#include <ostream>
#include "Vector.h"

struct Vertex
{
	Vertex();
	Vertex(Vector3 position, Vector3 normal);
	
	Vector3 position;
	Vector3 normal;
};

std::ostream& operator<<(std::ostream &stream, const Vertex &vert);