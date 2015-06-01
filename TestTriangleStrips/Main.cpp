#include "TestTriangleStripsDX.h"
#include "TestTriangleStripsGL.h"

const bool BENCH = true;
const int BENCH_FRAME_COUNT = 1000;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestTriangleStripsDX* dxApp = new TestTriangleStripsDX(hInstance);
	dxApp->debug = false;
	dxApp->benchmarking = BENCH;
	dxApp->processInput = !BENCH;
	dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	dxApp->Run();
	delete dxApp;

	TestTriangleStripsGL* glApp = new TestTriangleStripsGL(hInstance);
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
//const int DIM_X = 1800;
//const int DIM_Y = 1800;
//const float SIZE_X = 20;
//const float SIZE_Y = 20;
//
//int main()
//{
//	std::vector<Vector4> vertices;
//	for (int y = 0; y < DIM_Y; ++y)
//	{
//		for (int x = 0; x < DIM_X; ++x)
//		{
//			Vector4 v1;
//			v1.x = -(SIZE_X / 2.0f) + ((SIZE_X / DIM_X) / 2.0f) + (x * (SIZE_X / DIM_X));
//			v1.y = 0.0f;
//			v1.z = -(SIZE_Y / 2.0f) + ((SIZE_Y / DIM_Y) / 2.0f) + (y * (SIZE_Y / DIM_Y));
//			v1.w = 1.0f;
//
//			Vector4 v2;
//			v2.x = -(SIZE_X / 2.0f) + ((SIZE_X / DIM_X) / 2.0f) + (x * (SIZE_X / DIM_X));
//			v2.y = 0.0f;
//			v2.z = -(SIZE_Y / 2.0f) + ((SIZE_Y / DIM_Y) / 2.0f) + ((y + 1) * (SIZE_Y / DIM_Y));
//			v2.w = 1.0f;
//
//			vertices.push_back(v1);
//			vertices.push_back(v2);
//		}
//	}
//	BinaryIO::WriteVector4s("triangle_strip_plane.bin", vertices);
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}