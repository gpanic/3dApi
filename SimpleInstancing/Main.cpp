#include "SimpleInstancingDX.h"
#include "SimpleInstancingGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleInstancingDX dxApp(hInstance);
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	SimpleInstancingGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}