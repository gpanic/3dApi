#pragma once

namespace Data
{
	const float vertices[] =
	{
		-0.3f, -0.3f, 0.0f, 1.0f,
		-0.3f, 0.3f, 0.0f, 1.0f,
		0.3f, 0.3f, 0.0f, 1.0f,
		0.3f, -0.3f, 0.0f, 1.0f
	};
	const unsigned int instanceCount = 4;
	const float instanceData[] =
	{
		-0.5f, 0.5f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.5f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f

	};
	const unsigned int indexCount = 6;
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
};