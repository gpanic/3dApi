#pragma once
#include "GLApp.h"

class TestUniformUpdateGL : public GLApp
{
public:
	TestUniformUpdateGL(HINSTANCE hInstance);
	virtual ~TestUniformUpdateGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

