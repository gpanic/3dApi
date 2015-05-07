#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "BinaryIO.h"

class TestDrawGL : public GLApp
{
public:
	TestDrawGL(HINSTANCE hInstance);
	virtual ~TestDrawGL();
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
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 eye = glm::vec3(2.2f, 2.2f, 2.2f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

	const int DIMENSION_SIZE = 100.0f;
	std::vector<Vector4> verts;
};

