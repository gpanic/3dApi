#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "ObjReader.h"
#include "ModelDX.h"
using namespace DirectX;

class TestSceneDX : public DXApp
{
public:
	TestSceneDX(HINSTANCE hInstance);;
	virtual ~TestSceneDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

