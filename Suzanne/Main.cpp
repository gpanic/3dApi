#include "SuzanneDX.h"
#include "SuzanneGL.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SuzanneDX dxApp(hInstance);
	dxApp.debug = false;
	dxApp.benchmarking = false;
	dxApp.processInput = true;
	dxApp.Run();

	SuzanneGL glApp(hInstance);
	glApp.benchmarking = false;
	glApp.processInput = true;
	glApp.Run();
}