#include "Material.h"

Material::Material() : ambient(Vector4()), diffuse(Vector4()), specular(Vector4()), shininess(0) {}
Material::Material(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}