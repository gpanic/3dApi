#pragma once
#include "DXApp.h"
#include "Data.h"

class SimpleIndexedDrawingDX : public DXApp
{
public:
	SimpleIndexedDrawingDX(HINSTANCE hInstance);;
	virtual ~SimpleIndexedDrawingDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

