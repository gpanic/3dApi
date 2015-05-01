#include "Vertex.h"

Vertex::Vertex() : position(Vector4()), normal(Vector3()) {};
Vertex::Vertex(Vector4 position, Vector3 normal) : position(position), normal(normal) {};


std::ostream& operator<<(std::ostream &stream, const Vertex &vert)
{
	return stream << vert.position.x << " " << vert.position.y << " " << vert.position.z << " ";
}