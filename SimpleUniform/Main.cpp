#include "SimpleUniformDX.h"
#include "SimpleUniformGL.h"

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleUniformDX dxApp(hInstance);
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	SimpleUniformGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}