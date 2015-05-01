#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "ObjReader.h"
#include "Material.h"
#include "Light.h"
#include "ModelGL.h"

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

