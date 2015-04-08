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