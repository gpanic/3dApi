#pragma once
#include <array>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float w,
		float u, float v) :
		position({ { x, y, z, w } }), uv({ { u, v } })
	{
	}
	std::array<float, 4> position;
	std::array<float, 2> uv;
};

namespace Data
{
	const unsigned int vertexCount = 4;
	const Vertex verticesDX[] =
	{
		Vertex(-0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(-0.75f, 0.75f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(0.75f, 0.75f, 0.0f, 1.0f, 1.0f, 0.0f),
		Vertex(0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f)
	};
	const Vertex verticesGL[] =
	{
		Vertex(-0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-0.75f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.75f, 0.75f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f)
	};
	const unsigned int indexCount = 6;
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
};