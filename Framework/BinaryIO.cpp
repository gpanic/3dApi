#include "BinaryIO.h"
#include <iostream>

void BinaryIO::WriteVertices(const std::string &path, const std::vector<Vertex> &vertices, const std::string &materialName)
{
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		unsigned int materialStringSize = materialName.size();
		file.write(reinterpret_cast<const char *>(&materialStringSize), sizeof(materialStringSize));
		file.write(materialName.c_str(), materialName.size());
		file.write(reinterpret_cast<const char *>(&vertices[0]), vertices.size() * sizeof(Vertex));
	}
	file.close();
}

void BinaryIO::WriteVector4s(const std::string &path, const std::vector<Vector4> &vertices)
{
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		file.write(reinterpret_cast<const char *>(&vertices[0]), vertices.size() * sizeof(Vector4));
	}
	file.close();
}

void BinaryIO::ReadVector4s(const std::string &path, std::vector<Vector4> &vertices)
{
	std::ifstream file(path, std::ios::in | std::ios::ate | std::ios::binary);
	if (file.is_open())
	{
		long long size = file.tellg() / sizeof(Vector4);
		vertices.resize(size);
		file.seekg(0);
		file.read(reinterpret_cast<char *>(&vertices[0]), size * sizeof(Vector4));
	}
	file.close();
}