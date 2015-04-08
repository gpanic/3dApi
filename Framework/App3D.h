#pragma once
#include <windows.h>
#include <string>
using namespace std;

class App3D
{
public:
	App3D(HINSTANCE hInstance);
	virtual ~App3D();

	int Run();
	virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND			mWindow;
	HINSTANCE		mInstance;
	unsigned int	mHeight;
	unsigned int	mWidth;
	string			mAppTitle;
	string			mWindowClass;
	unsigned long	mWindowStyle;
	
	bool InitWindow();
	virtual bool InitAPI() = 0;
	virtual bool InitScene() = 0;
	int MsgLoop();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual void SwapBuffer() = 0;

};