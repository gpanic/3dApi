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
//
//int main()
//{
//	std::vector<Vertex> verts;
//	ObjReader::Read("sphere.obj", verts);
//	for (Vertex v : verts)
//	{
//		std::cout << "P " << v << " N " << v.normal.x << " " << v.normal.y << " " << v.normal.z << std::endl;
//	}
//	std::cout << verts.size() << std::endl;
//	std::cin.get();
//}