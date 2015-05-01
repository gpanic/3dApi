#include "ObjReader.h"

void ObjReader::ReadObj(const std::string &objPath, std::vector<Vertex> &verts, std::string &materialName)
{
	verts.clear();
	materialName.clear();
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;

	std::ifstream file(objPath);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::stringstream stream(line);
			std::string word;
			stream >> word;
			if (word == "v")
			{
				positions.push_back(Vector4(ExtractVector3(stream), 1.0f));
			}
			else if (word == "vn")
			{
				normals.push_back(ExtractVector3(stream));
			}
			else if (word == "usemtl")
			{
				stream >> materialName;
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
	file.close();
}

void ObjReader::ReadMtl(const std::string &mtlPath, const std::string &materialName, Material &material)
{
	bool foundMaterial = false;
	std::ifstream file(mtlPath);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::stringstream stream(line);
			std::string word;
			stream >> word;

			if (word == "newmtl")
			{
				foundMaterial = false;
				stream >> word;
				if (word == materialName)
				{
					foundMaterial = true;
				}
			}

			if (foundMaterial)
			{
				if (word == "Ns")
				{
					stream >> word;
					material.shininess = std::stof(word);
				}
				else if (word == "Ka")
				{
					material.ambient = Vector4(ExtractVector3(stream), 1.0f);
				}
				else if (word == "Kd")
				{
					material.diffuse = Vector4(ExtractVector3(stream), 1.0f);
				}
				else if (word == "Ks")
				{
					material.specular = Vector4(ExtractVector3(stream), 1.0f);
				}
			}
		}
	}
	file.close();
}

void ObjReader::Read(const std::string &objPath, const std::string &mtlPath, std::vector<Vertex> &verts, Material &material)
{
	std::string materialName;
	ReadObj(objPath, verts, materialName);
	ReadMtl(mtlPath, materialName, material);
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

Vertex ObjReader::ExtractVertex(std::stringstream &stream, std::vector<Vector4> &positions, std::vector<Vector3> &normals)
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