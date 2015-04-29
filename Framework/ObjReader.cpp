#include "ObjReader.h"

void ObjReader::Read(const std::string &path, std::vector<Vertex> &verts)
{
	verts.clear();
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;

	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream stream(line);
		std::string word;
		stream >> word;
		if (word == "v")
		{
			positions.push_back(ExtractVector3(stream));
		}
		else if (word == "vn")
		{
			normals.push_back(ExtractVector3(stream));
		}
		else if (word == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				verts.push_back(ExtractVertex(stream, positions, normals));
			}
		}
	}
}

Vector3 ObjReader::ExtractVector3(std::stringstream &stream)
{
	std::string word;
	Vector3 vec3;
	stream >> word;
	vec3.x = std::stof(word);
	stream >> word;
	vec3.y = std::stof(word);
	stream >> word;
	vec3.z = std::stof(word);
	return vec3;
}

Vertex ObjReader::ExtractVertex(std::stringstream &stream, std::vector<Vector3> &positions, std::vector<Vector3> &normals)
{
	std::string word;
	stream >> word;

	Vertex vert;
	std::string::size_type pos = 0;
	std::string token;

	pos = word.find('/');
	token = word.substr(0, pos);
	vert.position = positions[std::stoi(token) - 1];
	word.erase(0, pos + 1);

	pos = word.find('/');
	word.erase(0, pos + 1);

	pos = word.find('/');
	token = word.substr(0, pos);
	vert.normal = normals[std::stoi(token) - 1];
	word.erase(0, pos + 1);

	return vert;
}