#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "ObjReader.h"
#include "BinaryIO.h"
#include "Material.h"
#include "Lighting.h"

class TestTriangleStripsGL : public GLApp
{
public:
	TestTriangleStripsGL(HINSTANCE hInstance);
	virtual ~TestTriangleStripsGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	GLuint shaderProgram;
	GLuint vertexArray;

	std::vector<Vector4> vertices;

	GLuint lightBindingPoint = 0;

	GLuint modelMatrixIndex;
	GLuint viewMatrixIndex;
	GLuint projectionMatrixIndex;

	const float rotDelta = 5.0f;
	glm::vec3 up;
	glm::vec3 eye;
	glm::vec3 right;
	glm::vec3 center;
};

