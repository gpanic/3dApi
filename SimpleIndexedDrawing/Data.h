#pragma once
#include <array>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float w,
		float r, float g, float b, float a) :
		position({ { x, y, z, w } }), color({ { r, g, b, a } })
	{
	}
	std::array<float, 4> position;
	std::array<float, 4> color;
};

namespace Data
{
	const unsigned int vertexCount = 4;
	const Vertex vertices[] =
	{
		Vertex(-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.75f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.75f, 0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f)
	};
	const unsigned int indexCount = 6;
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
};