#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "Vertex.h"

namespace BinaryIO
{
	void WriteVertices(const std::string &path, const std::vector<Vertex> &vertices);
	void ReadVertices(const std::string &path, std::vector<Vertex> &vertices);
};

