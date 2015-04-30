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
//int main()
//{
//	std::vector<Vertex> verts;
//	ObjReader::Read("monkey.obj", verts);
//	BinaryIO::WriteVertices("monkey.bin", verts);
//	//BinaryIO::ReadVertices("sphere.bin", verts);
//	//for (Vertex v : verts)
//	//{
//	//	std::cout << v << std::endl;
//	//}
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}