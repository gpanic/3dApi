#pragma once
#include "GLApp.h"

class SimpleGL : public GLApp
{
public:
	SimpleGL(HINSTANCE hInstance);
	virtual ~SimpleGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

