#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "BinaryIO.h"

class TestPointsDX : public DXApp
{
public:
	TestPointsDX(HINSTANCE hInstance);;
	virtual ~TestPointsDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

