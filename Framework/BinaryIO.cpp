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

void BinaryIO::ReadVertices(const std::string &path, std::vector<Vertex> &vertices, std::string &materialName)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		unsigned int materialStringSize = 0;
		file.read(reinterpret_cast<char *>(&materialStringSize), sizeof(materialStringSize));
		materialName.resize(materialStringSize, ' ');
		file.read(reinterpret_cast<char *>(&materialName[0]), materialStringSize);
		std::fstream::pos_type pos = file.tellg();
		file.seekg(0, std::ios::end);
		long long size = (file.tellg() - pos) / sizeof(Vertex);
		vertices.resize(size);
		file.seekg(pos);
		file.read(reinterpret_cast<char *>(&vertices[0]), size * sizeof(Vertex));
		//long long size = file.tellg() / sizeof(Vertex);
		//vertices.resize(size);
		//file.seekg(0);
		//file.read(reinterpret_cast<char *>(&vertices[0]), size * sizeof(Vertex));
	}
	file.close();
}