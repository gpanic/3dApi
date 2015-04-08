#pragma once
#include "GLApp.h"

class SimpleGL : public GLApp
{
public:
	SimpleGL(HINSTANCE hInstance);
	virtual ~SimpleGL();
private:
	bool InitScene() override;
	void Update(float dt) override;
	void Render(float dt) override;
};

