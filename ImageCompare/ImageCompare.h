#pragma once
#include <string>
#include <Windows.h>
#include <fstream>

class ImageCompare
{
public:
	static float Compare(std::string img1, std::string img2, std::string diffOut);
};