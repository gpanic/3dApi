#include "Vertex.h"

Vector3::Vector3() : x(0), y(0), z(0) {};
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

Vertex::Vertex() : position(Vector3()), normal(Vector3()) {};
Vertex::Vertex(Vector3 position, Vector3 normal) : position(position), normal(normal) {};


std::ostream& operator<<(std::ostream &stream, const Vertex &vert)
{
	return stream << vert.position.x << " " << vert.position.y << " " << vert.position.z << " ";
}