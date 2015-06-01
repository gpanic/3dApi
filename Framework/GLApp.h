#pragma once
#include "App3D.h"
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glext.h>
#include <wglext.h>
#include "GLUtil.h"

class GLApp : public App3D
{
public:
	GLApp(HINSTANCE hInstance);
	virtual ~GLApp();

	bool vSync;

protected:
	HDC		mDeviceContext;
	HGLRC	mGLRenderContext;

	virtual bool InitScene() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = nullptr;
	PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = nullptr;

	bool InitAPI() override;
	void UpdateWindowTitle() override;
	void SwapBuffer() override;
	void SaveSnapshot(std::string file) override;
	
	bool WGLExtSupported(std::string extName);
	void SetVsync();
};

