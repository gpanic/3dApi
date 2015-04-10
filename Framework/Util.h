#pragma once
#include <string>
#include <fstream>
using namespace std;

struct Util
{
	inline static string ReadFile(const string &path)
	{
		ifstream file(path);
		string fileStr((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
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