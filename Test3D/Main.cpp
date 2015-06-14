#include "TestSceneDX.h"
#include "TestSceneGL.h"
#include "TestTriangleStripsDX.h"
#include "TestTriangleStripsGL.h"
#include "TestRasterizationDX.h"
#include "TestRasterizationGL.h"
#include "TestDrawDX.h"
#include "TestDrawGL.h"
#include "TestPointsDX.h"
#include "TestPointsGL.h"
#include "TestUniformUpdateDX.h"
#include "TestUniformUpdateGL.h"

const bool BENCH = true;
const bool DEBUG = false;
const int BENCH_FRAME_COUNT = 1000;

const bool TEST_SCENE = true;
const bool TEST_TRIANGLE_STRIPS = true;
const bool TEST_RASTERIZATION = true;
const bool TEST_DRAW = true;
const bool TEST_POINTS = true;
const bool TEST_UNIFORM_UPDATE = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (TEST_SCENE)
	{
		TestSceneDX* dxApp = new TestSceneDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->saveDeviceInfo = true;
		dxApp->Run();
		delete dxApp;

		TestSceneGL* glApp = new TestSceneGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->saveDeviceInfo = true;
		glApp->Run();
		delete glApp;
	}

	if (TEST_TRIANGLE_STRIPS)
	{
		TestTriangleStripsDX* dxApp = new TestTriangleStripsDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->Run();
		delete dxApp;

		TestTriangleStripsGL* glApp = new TestTriangleStripsGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->Run();
		delete glApp;
	}

	if (TEST_RASTERIZATION)
	{
		TestRasterizationDX* dxApp = new TestRasterizationDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->Run();
		delete dxApp;

		TestRasterizationGL* glApp = new TestRasterizationGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->Run();
		delete glApp;
	}

	if (TEST_DRAW)
	{
		TestDrawDX* dxApp = new TestDrawDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->Run();
		delete dxApp;

		TestDrawGL* glApp = new TestDrawGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->Run();
		delete glApp;
	}

	if (TEST_POINTS)
	{
		TestPointsDX* dxApp = new TestPointsDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->Run();
		delete dxApp;

		TestPointsGL* glApp = new TestPointsGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->Run();
		delete glApp;
	}

	if (TEST_UNIFORM_UPDATE)
	{
		TestUniformUpdateDX* dxApp = new TestUniformUpdateDX(hInstance);
		dxApp->debug = DEBUG;
		dxApp->benchmarking = BENCH;
		dxApp->processInput = !BENCH;
		dxApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		dxApp->Run();
		delete dxApp;

		TestUniformUpdateGL* glApp = new TestUniformUpdateGL(hInstance);
		glApp->benchmarking = BENCH;
		glApp->processInput = !BENCH;
		glApp->benchmarkFrameCount = BENCH_FRAME_COUNT;
		glApp->Run();
		delete glApp;
	}

	MessageBox(NULL, "FINISHED TESTING!", "3DAPI Test", MB_OK);
}