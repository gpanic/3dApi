#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include "ObjReader.h"

class TestSceneGL : public GLApp
{
public:
	TestSceneGL(HINSTANCE hInstance);
	virtual ~TestSceneGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

