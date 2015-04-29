#pragma once
#include <ostream>

struct Vector3
{
	Vector3();
	Vector3(float x, float y, float z);

	float x;
	float y;
	float z;
};

struct Vertex
{
	Vertex();
	Vertex(Vector3 position, Vector3 normal);
	
	Vector3 position;
	Vector3 normal;
};

std::ostream& operator<<(std::ostream &stream, const Vertex &vert);