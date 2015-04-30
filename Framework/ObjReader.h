#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Vertex.h"
#include "Material.h"

class ObjReader
{
public:
	static void ReadObj(const std::string &objPath, std::vector<Vertex> &verts, std::string &materialName);
	static void ReadMtl(const std::string &mtlPath, const std::string &materialName, Material &material);
	static void Read(const std::string &objPath, const std::string &mtlPath, std::vector<Vertex> &verts, Material &material);

private:
	static Vector3 ExtractVector3(std::stringstream &stream);
	static Vertex ExtractVertex(std::stringstream &stream, std::vector<Vector3> &positions, std::vector<Vector3> &normals);
};

