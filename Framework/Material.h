#pragma once
#include "Vector.h"

struct Material
{
	Material();
	Material(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shininess);
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	float shininess;
	Vector3 padding;
};

