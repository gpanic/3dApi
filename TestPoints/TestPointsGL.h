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

	GLuint shaderProgram;
	GLuint vao;
	GLuint vbo;

	GLuint modelMatrixIndex;
	GLuint viewMatrixIndex;
	GLuint projectionMatrixIndex;

	const float rotDelta = 5.0f;
	glm::vec3 up;
	glm::vec3 eye;
	glm::vec3 right;
	glm::vec3 center;

	const int DIMENSION_SIZE = 100.0f;
	std::vector<Vector4> verts;
};

