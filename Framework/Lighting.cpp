#include "Lighting.h"

Light::Light() : position(Vector4()), diffuse(Vector4()), specular(Vector4()) {}
Light::Light(Vector4 position, Vector4 diffuse, Vector4 specular, float halfDistance) : position(position), diffuse(diffuse), specular(specular), halfDistance(halfDistance) {}

Lighting::Lighting() : ambient(Vector4()) {};
Lighting::Lighting(Vector4 ambient) : ambient(ambient) {};