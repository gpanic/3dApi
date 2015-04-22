#pragma once
#include "GLApp.h"
#include "Data.h"

class SimpleTexturingGL : public GLApp
{
public:
	SimpleTexturingGL(HINSTANCE hInstance);
	virtual ~SimpleTexturingGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

