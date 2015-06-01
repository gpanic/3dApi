#include "TestSceneDX.h"
#include "TestSceneGL.h"

const bool BENCH = true;
const int BENCH_FRAME_COUNT = 1000;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestSceneDX* dxApp = new TestSceneDX(hInstance);
	dxApp->debug = false;
	dxApp->benchmarking = BENCH;
	dxApp->processInput = !BENCH;
	dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	dxApp->Run();
	delete dxApp;

	TestSceneGL* glApp = new TestSceneGL(hInstance);
	glApp->benchmarking = BENCH;
	glApp->processInput = !BENCH;
	glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	glApp->Run();
	delete glApp;
}

//#include <iostream>
//#include <vector>
//#include "ObjReader.h"
//#include "BinaryIO.h"
//
//const std::vector<std::string> objs = { "monkey.obj"/*, "knife.obj", "sphere_smooth.obj", "monkey.obj", "plane.obj" */ };
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