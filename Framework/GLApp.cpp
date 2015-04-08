#include "GLApp.h"

GLApp::GLApp(HINSTANCE hInstance) : App3D(hInstance)
{
	mAppTitle = "OpenGL App";
	mWindowClass = "GLAPPWNDCLASS";

	mDeviceContext = nullptr;
	mGLRenderContext = nullptr;
}

GLApp::~GLApp()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(mGLRenderContext);
	ReleaseDC(mWindow, mDeviceContext);
}

bool GLApp::InitAPI()
{
	mDeviceContext = GetDC(mWindow);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int format = ChoosePixelFormat(mDeviceContext, &pfd);
	if (!SetPixelFormat(mDeviceContext, format, &pfd))
	{
		MessageBox(NULL, "Error setting pixel format", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	mGLRenderContext = wglCreateContext(mDeviceContext);
	if (!wglMakeCurrent(mDeviceContext, mGLRenderContext))
	{
		MessageBox(NULL, "Error creating and activatign render context", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (glewInit())
	{
		MessageBox(NULL, "Error initializing GLEW", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

void GLApp::SwapBuffer()
{
	SwapBuffers(mDeviceContext);
}