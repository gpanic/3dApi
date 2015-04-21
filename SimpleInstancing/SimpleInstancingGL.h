#pragma once
#include "GLApp.h"
#include "Data.h"

class SimpleInstancingGL : public GLApp
{
public:
	SimpleInstancingGL(HINSTANCE hInstance);
	virtual ~SimpleInstancingGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

