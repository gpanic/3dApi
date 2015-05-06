#include "TestPointsDX.h"
#include "TestPointsGL.h"
const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TestPointsDX dxApp(hInstance);
	dxApp.debug = true;
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	//TestPointsGL glApp(hInstance);
	//glApp.benchmarking = BENCH;
	//glApp.Run();
}

//const int DIMENSION_SIZE = 200.0f;
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
//				float x = -1.0f + ((i + 1) * 2.0f / DIMENSION_SIZE);
//				float y = -1.0f + ((j + 1) * 2.0f / DIMENSION_SIZE);
//				float z = -1.0f + ((k + 1) * 2.0f / DIMENSION_SIZE);
//				verts.push_back(Vector4(x, y, z, 1.0f));
//			}
//		}
//	}
//
//	BinaryIO::WriteVector4s("point_cube.bin", verts);
//
//	std::cout << std::endl;
//	std::cout << "AAAAAAA" << std::endl;
//	std::cin.get();
//}