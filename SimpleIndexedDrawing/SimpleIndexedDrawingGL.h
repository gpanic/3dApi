#pragma once
#include "GLApp.h"
#include "Data.h"

class SimpleIndexedDrawingGL : public GLApp
{
public:
	SimpleIndexedDrawingGL(HINSTANCE hInstance);
	virtual ~SimpleIndexedDrawingGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

