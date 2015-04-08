#include "App3D.h"

namespace
{
	App3D* gApp = NULL;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gApp)
		return gApp->WndProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

App3D::App3D(HINSTANCE hInstance)
{
	mInstance = hInstance;
	mWindow = NULL;
	mWidth = 800;
	mHeight = 800;
	mAppTitle = "3D App";
	mWindowClass = "3DAPPWNDCLASS";
	mWindowStyle = WS_OVERLAPPEDWINDOW;
	gApp = this;
}

App3D::~App3D()
{
}

int App3D::Run()
{
	if (!InitWindow())
		return 1;
	if (!InitAPI())
		return 1;
	if (!InitScene())
		return 1;
	return MsgLoop();
}

int App3D::MsgLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(0.0f);
			Render(0.0f);
			SwapBuffer();
		}
	}
	return static_cast<int>(msg.wParam);
}

bool App3D::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mInstance;
	wcex.hIcon = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = mWindowClass.c_str();
	wcex.hIconSm = LoadIcon(mInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	RECT r = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&r, mWindowStyle, FALSE);
	unsigned int width = r.right - r.left;
	unsigned int height = r.bottom - r.top;
	unsigned int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	unsigned int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	mWindow = CreateWindow(
		mWindowClass.c_str(),
		mAppTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		x, y,
		width, height,
		NULL,
		NULL,
		mInstance,
		NULL
		);

	if (!mWindow)
	{
		MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(mWindow, SW_SHOW);
	UpdateWindow(mWindow);

	return true;
}

LRESULT App3D::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}