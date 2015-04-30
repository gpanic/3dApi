#pragma once
#include "Vector.h"

struct Light
{
	Light();
	Light(Vector4 position, Vector4 ambient, Vector4 diffuse, Vector4 specular);
	Vector4 position;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};
