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
	gApp = this;

	mInstance = hInstance;
	mWindow = NULL;
	mWidth = 800;
	mHeight = 800;
	mAppTitle = "3D App";
	mWindowClass = "3DAPPWNDCLASS";
	mWindowStyle = WS_OVERLAPPEDWINDOW;
	mBenchmarkResultName = mAppTitle + "Result.txt";

	benchmarking = true;
	benchmarkFrameCount = 20000;
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



int App3D::MsgLoop()
{
	StartTime();

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
			UpdateDeltaTime();
			Update();
			Render();
			UpdateWindowTitle();
			SwapBuffer();
			UpdateTime();
			if (!Benchmark())
				break;
		}
	}
	return static_cast<int>(msg.wParam);
}

void App3D::StartTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mPreviousCount);
	QueryPerformanceFrequency((LARGE_INTEGER*)&mFrequency);
}

void App3D::UpdateDeltaTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCount);
	mDeltaTime = (float)(mCurrentCount - mPreviousCount) / (float)mFrequency;

	mElapsedTime += mDeltaTime;
	++mDeltaFrameCount;
	if (mElapsedTime >= 1.0f)
	{
		mFPS = (float)mDeltaFrameCount;
		mElapsedTime -= 1.0f;
		mDeltaFrameCount = 0;
	}
}

void App3D::UpdateTime()
{
	mPreviousCount = mCurrentCount;
}

bool App3D::Benchmark()
{
	if (benchmarking)
	{
		if (mFrameTimes.size() != benchmarkFrameCount)
			mFrameTimes.resize(benchmarkFrameCount);

		mFrameTimes[mFrameCount] = mDeltaTime;
		++mFrameCount;
		if (mFrameCount == benchmarkFrameCount)
		{
			std::ofstream file;
			file.open(mBenchmarkResultName);
			float timeSum = 0;
			for (int j = 0; j < benchmarkFrameCount; ++j)
			{
				float time = mFrameTimes[j];
				timeSum += time;
				file << std::setfill('0') << std::setw(2) << j << " " << std::fixed << time << std::endl;
			}
			file << "TIME " << timeSum << std::endl;
			file << "FPS " << (float)benchmarkFrameCount / timeSum << std::endl;
			file.close();
			return false;
		}
	}
	return true;
}