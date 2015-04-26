#include "ObjReader.h"

void ObjReader::Read(const std::string &path, std::vector<Vertex> &verts, std::vector<Face> &faces)
{
	verts.clear();
	faces.clear();

	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{	
		std::stringstream stream(line);
		std::string word;
		stream >> word;
		if (word == "v")
		{
			stream >> word;
			float x = std::stof(word);
			stream >> word;
			float y = std::stof(word);
			stream >> word;
			float z = std::stof(word);
			verts.push_back(Vertex(x, y, z));
		}
		else if (word == "f")
		{
			stream >> word;
			unsigned short v1 = word[0] - '0' - 1;
			stream >> word;
			unsigned short v2 = word[0] - '0' - 1;
			stream >> word;
			unsigned short v3 = word[0] - '0' - 1;
			faces.push_back(Face(v1, v2, v3));
		}
	}
}