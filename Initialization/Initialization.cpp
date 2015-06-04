#include "Initialization.h"

bool Initialization::GenerateModelBinary(std::initializer_list<std::string> objs)
{
	std::vector<Vertex> verts;
	Material mat;
	std::string materialName;

	for (std::string obj : objs)
	{
		std::string s = obj;
		s.erase(s.length() - 4, 4);
		s.append(".bin");
		ObjReader::ReadObj(obj, verts, materialName);
		BinaryIO::WriteVertices(s, verts, materialName);
	}
	return true;
}

bool Initialization::GenerateTrinagleStrips(int dimX, int dimY, float sizeX, float sizeY, std::string fileOut)
{
	std::vector<Vector4> vertices;
	for (int y = 0; y < dimY; ++y)
	{
		for (int x = 0; x < dimX; ++x)
		{
			Vector4 v1;
			v1.x = -(sizeX / 2.0f) + ((sizeX / dimX) / 2.0f) + (x * (sizeX / dimX));
			v1.y = 0.0f;
			v1.z = -(sizeY / 2.0f) + ((sizeY / dimY) / 2.0f) + (y * (sizeY / dimY));
			v1.w = 1.0f;

			Vector4 v2;
			v2.x = -(sizeX / 2.0f) + ((sizeX / dimX) / 2.0f) + (x * (sizeX / dimX));
			v2.y = 0.0f;
			v2.z = -(sizeY / 2.0f) + ((sizeY / dimY) / 2.0f) + ((y + 1) * (sizeY / dimY));
			v2.w = 1.0f;

			vertices.push_back(v1);
			vertices.push_back(v2);
		}
	}
	BinaryIO::WriteVector4s(fileOut, vertices);
	return true;
}

bool Initialization::GenerateInstanceOffsets(int dimension, float size, std::string fileOut)
{
	std::vector<Vector3> verts;
	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
			verts.push_back(Vector3(-(size / 2) + (size / dimension / 2.0f) + (i * dimension / size), 0.0f, (size / 2) - (size / dimension / 2.0f) - (j * dimension / size)));
		}
	}
	BinaryIO::WriteVector3s(fileOut, verts);
	return true;
}

bool Initialization::GeneratePointCube(int dimension, float size, std::string fileOut)
{
	std::vector<Vector4> verts;
	for (int i = 0; i < dimension; ++i)
	{
		std::cout << ".";
		for (int j = 0; j < dimension; ++j)
		{
			for (int k = 0; k < dimension; ++k)
			{
				float x = -(size / 2.0f) + ((size / 2.0f) / dimension) + (i * size / dimension);
				float y = -(size / 2.0f) + ((size / 2.0f) / dimension) + (j * size / dimension);
				float z = -(size / 2.0f) + ((size / 2.0f) / dimension) + (k * size / dimension);
				verts.push_back(Vector4(x, y, z, 1.0f));
			}
		}
	}
	BinaryIO::WriteVector4s(fileOut, verts);
	std::cout << std::endl;
	return true;
}