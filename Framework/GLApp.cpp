#include "GLApp.h"

GLApp::GLApp(HINSTANCE hInstance) : App3D(hInstance)
{
	mAppTitle = "OpenGL App";
	mWindowClass = "GLAPPWNDCLASS";

	mDeviceContext = nullptr;
	mGLRenderContext = nullptr;

	vSync = false;
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

	glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);

	SetVsync();

	return true;
}

void GLApp::UpdateWindowTitle()
{
	std::stringstream ss;
	ss << mAppTitle << " | FPS " << mFPS;
	SetWindowText(mWindow, ss.str().c_str());
}

void GLApp::SwapBuffer()
{
	SwapBuffers(mDeviceContext);
}

void GLApp::SaveSnapshot(std::string filePath)
{
	byte *bmpBuffer = new byte[mWidth * mHeight * 3];
	if (!bmpBuffer) return;

	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, mWidth - 1, mHeight - 1, GL_BGR, GL_UNSIGNED_BYTE, bmpBuffer);

	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	if (!file.is_open()) return;

	BITMAPFILEHEADER bitmapFileHeader;
	bitmapFileHeader.bfType = 0x4D42; //"BM"
	bitmapFileHeader.bfSize = mWidth * mHeight * 3;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bitmapInfoHeader;
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = mWidth - 1;
	bitmapInfoHeader.biHeight = mHeight - 1;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = 0;
	bitmapInfoHeader.biXPelsPerMeter = 0; // ?
	bitmapInfoHeader.biYPelsPerMeter = 0; // ?
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;

	file.write(reinterpret_cast<char *>(&bitmapFileHeader), sizeof(bitmapFileHeader));
	file.write(reinterpret_cast<char *>(&bitmapInfoHeader), sizeof(bitmapInfoHeader));
	file.write(reinterpret_cast<char *>(bmpBuffer), mWidth * mHeight * 3);
	file.close();

	delete[] bmpBuffer;
}

bool GLApp::WGLExtSupported(std::string extName)
{
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;
	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	return !(strstr(_wglGetExtensionsStringEXT(), extName.c_str()) == NULL);
}

void GLApp::SetVsync()
{
	if (WGLExtSupported("WGL_EXT_swap_control"))
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		wglSwapIntervalEXT(vSync);
	}
}