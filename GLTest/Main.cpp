#include "SimpleGL.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleGL glApp(hInstance);
	glApp.Run();
}