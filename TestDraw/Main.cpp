#include "TestDrawDX.h"
#include "TestDrawGL.h"

const bool BENCH = false;
const int BENCH_FRAME_COUNT = 1000;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestDrawDX* dxApp = new TestDrawDX(hInstance);
	dxApp->debug = false;
	dxApp->benchmarking = BENCH;
	dxApp->processInput = !BENCH;
	dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	dxApp->Run();
	delete dxApp;

	TestDrawGL* glApp = new TestDrawGL(hInstance);
	glApp->benchmarking = BENCH;
	glApp->processInput = !BENCH;
	glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
	glApp->Run();
	delete glApp;
}
//
//const int DIMENSION_SIZE = 50.0f;
//int main()
//{
//	std::vector<Vector4> verts;
//	for (int i = 0; i < DIMENSION_SIZE; ++i)
//	{
//		std::cout << ".";
//		for (int j = 0; j < DIMENSION_SIZE; ++j)
//		{
//			for (int k = 0; k < DIMENSION_SIZE; ++k)
//			{
//				float x = -1.0f + (1.0f / DIMENSION_SIZE) + (i * 2.0f / DIMENSION_SIZE);
//				float y = -1.0f + (1.0f / DIMENSION_SIZE) + (j * 2.0f / DIMENSION_SIZE);
//				float z = -1.0f + (1.0f / DIMENSION_SIZE) + (k * 2.0f / DIMENSION_SIZE);
//				verts.push_back(Vector4(x, y, z, 1.0f));
//			}
//		}
//	}
//
//	BinaryIO::WriteVector4s("point_cube.bin", verts);
//
//	std::cout << std::endl;
//	std::cout << "DONE" << std::endl;
//	std::cin.get();
//}