#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "ObjReader.h"
#include "Material.h"
#include "Lighting.h"
#include "ModelGL.h"

class TestRasterizationGL : public GLApp
{
public:
	TestRasterizationGL(HINSTANCE hInstance);
	virtual ~TestRasterizationGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

