#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "BinaryIO.h"

class TestPointsGL : public GLApp
{
public:
	TestPointsGL(HINSTANCE hInstance);
	virtual ~TestPointsGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;
};

