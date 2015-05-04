#include "SimpleDX.h"
#include "SimpleGL.h"
// https://www.youtube.com/watch?v=6yghu5QUz50
// https://www.youtube.com/watch?v=xrYcu12BRL0

const bool BENCH = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleDX dxApp(hInstance);
	dxApp.benchmarking = BENCH;
	dxApp.Run();
	SimpleGL glApp(hInstance);
	glApp.benchmarking = BENCH;
	glApp.Run();
}