#pragma once
#include <string>
#include <vector>
#include "Vector.h"
#include "ObjReader.h"
#include "BinaryIO.h"

class Initialization
{
public:
	static bool GenerateModelBinary(std::initializer_list<std::string> objs);
	static bool GenerateTrinagleStrips(int dimX, int dimY, float sizeX, float sizeY, std::string fileOut);
	static bool GenerateInstanceOffsets(int dimension, float size, std::string fileOut);
	static bool GeneratePointCube(int dimension, float size, std::string fileOut);
};