#pragma once

struct Vector3
{
	Vector3();
	Vector3(float x, float y, float z);

	float x;
	float y;
	float z;
};

struct Vector4
{
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 vec3, float w);

	float x;
	float y;
	float z;
	float w;
};