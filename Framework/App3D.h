#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Util.h"
using namespace std;

class App3D
{
public:
	App3D(HINSTANCE hInstance);
	virtual ~App3D();

	bool benchmarking;
	int benchmarkFrameCount;
	Color bgColor;

	int Run();
	virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	double			mDeltaTime = 0;
	float			mFPS = 0;
	string			mBenchmarkResultName;
	HWND			mWindow;
	HINSTANCE		mInstance;
	unsigned int	mHeight;
	unsigned int	mWidth;
	string			mAppTitle;
	string			mWindowClass;
	unsigned long	mWindowStyle;

	virtual bool InitAPI() = 0;
	virtual bool InitScene() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void UpdateWindowTitle() = 0;
	virtual void SwapBuffer() = 0;

private:
	__int64			mPreviousCount = 0;
	__int64			mCurrentCount = 0;
	__int64			mFrequency = 0;
	float			mElapsedTime = 0;
	unsigned int	mDeltaFrameCount = 0;
	vector<float>	mFrameTimes;
	unsigned int	mFrameCount = 0;

	bool InitWindow();
	int MsgLoop();
	void StartTime();
	void UpdateDeltaTime();
	void UpdateTime();
	bool Benchmark();
};