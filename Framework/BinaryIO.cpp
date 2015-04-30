#include "BinaryIO.h"

void BinaryIO::WriteVertices(const std::string &path, const std::vector<Vertex> &vertices)
{
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		file.write(reinterpret_cast<const char *>(&vertices[0]), vertices.size() * sizeof(Vertex));
	}
	file.close();
}

void BinaryIO::ReadVertices(const std::string &path, std::vector<Vertex> &vertices)
{
	std::ifstream file(path, std::ios::in | std::ios::ate | std::ios::binary);
	if (file.is_open())
	{
		long long size = file.tellg() / sizeof(Vertex);
		file.seekg(0);
		vertices.clear();
		vertices.resize(size);
		file.read(reinterpret_cast<char *>(&vertices[0]), size * sizeof(Vertex));
	}
	file.close();
}