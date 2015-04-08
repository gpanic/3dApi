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

protected:
	HDC mDeviceContext;
	HGLRC mGLRenderContext;

	bool InitAPI() override;
	virtual bool InitScene() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	void SwapBuffer() override;
};

