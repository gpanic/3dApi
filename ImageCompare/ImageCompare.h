#pragma once
#include <string>
#include <Windows.h>
#include <fstream>

namespace ImageCompare
{
	struct Result
	{
		int samePixels;
		int differentPixels;
		int allPixels;
		float ratio;
	};

	Result Compare(std::string img1, std::string img2, std::string diffOut);
};