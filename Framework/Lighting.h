#pragma once
#include <vector>
#include "Vector.h"

struct Light
{
	Light();
	Light(Vector4 position, Vector4 diffuse, Vector4 specular, float halfDistance);
	Vector4 position;
	Vector4 diffuse;
	Vector4 specular;
	float halfDistance;
	Vector3 padding;
};

struct Lighting
{
	Lighting();
	Lighting(Vector4 ambient);

	Vector4 ambient;
	Light lights[2];
};