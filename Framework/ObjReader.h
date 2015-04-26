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
	static void Read(const std::string &path, std::vector<Vertex> &verts, std::vector<Face> &faces);
};

