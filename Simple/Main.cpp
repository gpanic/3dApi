#include "SimpleDX.h"
#include "SimpleGL.h"
// https://www.youtube.com/watch?v=6yghu5QUz50
// https://www.youtube.com/watch?v=xrYcu12BRL0

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleDX dxApp(hInstance);
	dxApp.Run();
	SimpleGL glApp(hInstance);
	glApp.Run();
}