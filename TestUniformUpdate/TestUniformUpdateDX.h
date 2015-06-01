#pragma once
#include "DXApp.h"

class TestUniformUpdateDX : public DXApp
{
public:
	TestUniformUpdateDX(HINSTANCE hInstance);;
	virtual ~TestUniformUpdateDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

