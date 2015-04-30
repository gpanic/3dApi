#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace Util
{
	inline std::string ReadFile(const std::string &path)
	{
		std::ifstream file(path);
		std::string fileStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return fileStr;
	}
};

struct Color
{
	Color() :
		r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float pr, float pg, float pb, float pa) :
		r(pr), g(pg), b(pb), a(pa) {}
	float r;
	float g;
	float b;
	float a;
};