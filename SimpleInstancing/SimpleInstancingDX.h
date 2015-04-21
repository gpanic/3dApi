#pragma once
#include "DXApp.h"
#include "Data.h"

class SimpleInstancingDX : public DXApp
{
public:
	SimpleInstancingDX(HINSTANCE hInstance);;
	virtual ~SimpleInstancingDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

