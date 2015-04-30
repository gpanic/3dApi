#include "Light.h"

Light::Light() : position(Vector4()), ambient(Vector4()), diffuse(Vector4()), specular(Vector4()) {}
Light::Light(Vector4 position, Vector4 ambient, Vector4 diffuse, Vector4 specular) : position(position), ambient(ambient), diffuse(diffuse), specular(specular) {}