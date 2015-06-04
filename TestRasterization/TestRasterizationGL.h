#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "ObjReader.h"
#include "BinaryIO.h"
#include "Material.h"
#include "Lighting.h"

class TestRasterizationGL : public GLApp
{
public:
	TestRasterizationGL(HINSTANCE hInstance);
	virtual ~TestRasterizationGL();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	GLuint shaderProgram;

	GLuint vertexArray;
	Material material;

	std::vector<Vertex> vertices;
	std::vector<Vector3> offsets;

	GLuint materialBuffer;
	GLuint materialBindingPoint = 1;
	GLuint lightBindingPoint = 2;

	GLuint modelMatrixIndex;
	GLuint viewMatrixIndex;
	GLuint projectionMatrixIndex;

	const float rotDelta = 5.0f;
	glm::vec3 up;
	glm::vec3 eye;
	glm::vec3 right;
	glm::vec3 center;
};

