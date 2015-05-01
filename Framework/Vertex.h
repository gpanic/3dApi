#pragma once
#include <ostream>
#include "Vector.h"

struct Vertex
{
	Vertex();
	Vertex(Vector4 position, Vector3 normal);
	
	Vector4 position;
	Vector3 normal;
};

std::ostream& operator<<(std::ostream &stream, const Vertex &vert);