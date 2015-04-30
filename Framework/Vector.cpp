#include "Vector.h"

Vector3::Vector3() : x(0), y(0), z(0) {};
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

Vector4::Vector4() : x(0), y(0), z(0), w(0) {};
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
Vector4::Vector4(Vector3 vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {};