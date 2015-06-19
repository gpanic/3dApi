#include <iostream>
#include <vector>
#include "ImageCompare.h"

std::string outPath = "Image Comparision/";

std::vector<std::string> testNames = {
	"test_draw",
	"test_points",
	"test_rasterization",
	"test_scene",
	"test_triangle_strips",
	"test_uniform_update"
};

int main()
{
	CreateDirectory(outPath.c_str(), NULL);
	std::string out = outPath + "img_comparison.txt";

	std::ofstream file(out);
	for (std::string test : testNames)
	{
		file << test << std::endl;
		//file << "dx_start_end " << ImageCompare::Compare("dx_" + test + "_start.bmp", "dx_" + test + "_end.bmp", outPath + "dx_" + test + "_diff.bmp") << std::endl;
		//file << "gl_start_end " << ImageCompare::Compare("gl_" + test + "_start.bmp", "gl_" + test + "_end.bmp", outPath + "gl_" + test + "_diff.bmp") << std::endl;
		file << "dx_end_gl_end " << ImageCompare::Compare("dx_" + test + "_end.bmp", "gl_" + test + "_end.bmp", outPath + test + "_diff.bmp") << std::endl;
		file << std::endl;
	}

	std::cout << "DONE" << std::endl;
	std::cin.get();
}