#pragma once
#include "GLApp.h"
#include "Data.h"

class SimpleUniformGL : public GLApp
{
public:
	SimpleUniformGL(HINSTANCE hInstance);
	virtual ~SimpleUniformGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

