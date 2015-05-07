#include "TestRasterizationDX.h"
#include "TestRasterizationGL.h"

const bool BENCH = false;
const int BENCH_FRAME_COUNT = 1000;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestRasterizationDX* dxApp = new TestRasterizationDX(hInstance);
	dxApp->debug = false;
	dxApp->benchmarking = BENCH;
	dxApp->processInput = !BENCH;
	dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	dxApp->Run();
	delete dxApp;

	TestRasterizationGL* glApp = new TestRasterizationGL(hInstance);
	glApp->benchmarking = BENCH;
	glApp->processInput = !BENCH;
	glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	glApp->Run();
	delete glApp;
}

//#include <iostream>
//#include <vector>
//#include "Vector.h"
//#include "BinaryIO.h"
//
//const int DIMENSION_SIZE = 100.0f;
//int main()
//{
//	std::vector<Vector3> verts;
//	for (int i = 0; i < DIMENSION_SIZE; ++i)
//	{
//		for (int j = 0; j < DIMENSION_SIZE; ++j)
//		{
//			verts.push_back(Vector3(-50.0f + 0.5f + (i * DIMENSION_SIZE / 100.0f), 0.0f, 50.0f - 0.5f - (j * DIMENSION_SIZE / 100.0f)));
//		}
//	}
//
//	BinaryIO::WriteVector3s("verts.bin", verts);
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}