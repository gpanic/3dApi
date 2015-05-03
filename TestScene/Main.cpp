#include "TestSceneDX.h"
#include "TestSceneGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestSceneDX dxApp(hInstance);
	dxApp.debug = true;
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	TestSceneGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}

//#include <iostream>
//#include <vector>
//#include "ObjReader.h"
//#include "BinaryIO.h"
//
//const std::vector<std::string> objs = { "torus.obj", "sphere.obj", "sphere_smooth.obj", "monkey.obj", "plane.obj" };
//
//int main()
//{
//	std::vector<Vertex> verts;
//	Material mat;
//	std::string materialName;
//
//	for (std::string obj : objs)
//	{
//		std::string s = obj;
//		s.erase(s.length() - 4, 4);
//		s.append(".bin");
//		ObjReader::ReadObj(obj, verts, materialName);
//		BinaryIO::WriteVertices(s, verts, materialName);
//	}
//
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}