#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Vertex.h"
#include "Face.h"

class ObjReader
{
public:
	static void Read(const std::string &path, std::vector<Vertex> &verts);
private:
	static Vector3 ExtractVector3(std::stringstream &stream);
	static Vertex ExtractVertex(std::stringstream &stream, std::vector<Vector3> &positions, std::vector<Vector3> &normals);
};

