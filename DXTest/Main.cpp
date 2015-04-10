#include "SimpleDX.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SimpleDX dxApp(hInstance);
	dxApp.Run();
}