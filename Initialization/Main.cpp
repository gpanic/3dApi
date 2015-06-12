#include "Initialization.h"

const bool TEST_SCENE = false;
const bool TEST_TRIANGLE_STRIPS = false;
const bool TEST_RASTERIZATION = false;
const bool TEST_DRAW = true;
const bool TEST_POINTS = true;

std::string assetPath = "../Assets/";
std::string shaderPath;
std::string binaryPath = assetPath + "Binary/";
std::string modelPath = assetPath + "Models/";

int main()
{
	std::cout << "START INITIALIZING" << std::endl;

	if (TEST_SCENE)
	{
		std::cout << "INIT TestScene" << std::endl;
		Initialization::GenerateModelBinary({
			modelPath + "sphere_smooth.obj",
			modelPath + "plane.obj",
			modelPath + "knife.obj",
			modelPath + "monkey.obj",
			modelPath + "chair.obj"
		});
	}

	if (TEST_TRIANGLE_STRIPS)
	{
		std::cout << "INIT TestTriangleStrips" << std::endl;
		Initialization::GenerateTrinagleStrips(1800, 1800, 20.0f, 20.0f, binaryPath + "triangle_strip_plane.bin");
	}

	if (TEST_RASTERIZATION)
	{
		std::cout << "INIT TestRasterization" << std::endl;
		Initialization::GenerateInstanceOffsets(100, 100.0f, binaryPath + "instance_offsets.bin");
	}

	if (TEST_DRAW)
	{
		std::cout << "INIT TestDraw" << std::endl;
		Initialization::GeneratePointCube(50, 2.0f, binaryPath + "point_cube_50.bin");
	}

	if (TEST_POINTS)
	{
		std::cout << "INIT TestPoints" << std::endl;
		Initialization::GeneratePointCube(180, 2.0f, binaryPath + "point_cube_180.bin");
	}

	std::cout << "DONE INITIALIZING" << std::endl;
	std::cin.get();
}