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
	const Vertex vertices[] =
	{
		Vertex(-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(0.0f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f)
	};
};