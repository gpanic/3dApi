#include "TestSceneGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//SimpleUniformDX dxApp(hInstance);
	//dxApp.benchmarking = BENCH;
	//dxApp.Run();
	TestSceneGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}

//#include <iostream>
//#include <vector>
//#include "ObjReader.h"
//#include "BinaryIO.h"
//
//const std::vector<std::string> objs = { "cube.obj", "sphere.obj", "sphere_smooth.obj", "monkey.obj", "plane.obj" };
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
//	//BinaryIO::ReadVertices("cube.bin", verts);
//	//std::cout << "ASHDASD" << std::endl;
//	//ObjReader::ReadMtl("cube.mtl", "None", mat);
//
//	//ObjReader::Read("cube.obj", "", verts, mat);
//	//BinaryIO::WriteVertices("cube.asdf", verts, "TESTNAME");
//
//	//std::string matName;
//	//BinaryIO::ReadVertices("cube.asdf", verts, matName);
//	//std::cout << matName << std::endl;
//
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}