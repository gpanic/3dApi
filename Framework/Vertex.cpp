#include "Vertex.h"

Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z), w(1) {};
Vertex::Vertex(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

std::ostream& operator<<(std::ostream &stream, const Vertex &vert)
{
	return stream << vert.x << " " << vert.y << " " << vert.z << " " << vert.w;
}