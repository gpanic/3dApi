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