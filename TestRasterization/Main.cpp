#include "TestRasterizationGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//SimpleDX dxApp(hInstance);
	//dxApp.benchmarking = BENCH;
	//dxApp.Run();
	TestRasterizationGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}