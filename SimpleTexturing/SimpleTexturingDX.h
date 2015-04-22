#pragma once
#include "DXApp.h"
#include "Data.h"
#include <DDSTextureLoader.h>

class SimpleTexturingDX : public DXApp
{
public:
	SimpleTexturingDX(HINSTANCE hInstance);;
	virtual ~SimpleTexturingDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

