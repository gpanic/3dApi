#include "SimpleTexturingDX.h"
#include "SimpleTexturingGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleTexturingDX dxApp(hInstance);
	dxApp.debug = true;
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	SimpleTexturingGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}