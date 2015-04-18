#pragma once
#include "DXApp.h"
#include "Data.h"

class SimpleUniformDX : public DXApp
{
public:
	SimpleUniformDX(HINSTANCE hInstance);;
	virtual ~SimpleUniformDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

