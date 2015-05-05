#pragma once
#include "GLApp.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "ObjReader.h"
#include "Material.h"
#include "Lighting.h"
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

	void RenderShadowMaps();

	static const int NUMBER_OF_LIGHTS = 2;

	GLuint shaderProgram;
	GLuint shadowShaderProgram;

	GLuint materialBindingPoint = 1;
	GLuint lightBindingPoint = 2;
	GLuint materialBuffer;

	GLuint modelMatrixIndex;
	GLuint viewMatrixIndex;
	GLuint projectionMatrixIndex;

	std::vector<ModelGL> models;

	const float rotDelta = 10.0f;
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 eye = glm::vec3(5.0f, 5.0f, 8.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

	// LIGHTING
	Lighting lighting;

	// SHADOW MAPPING
	const int SHADOW_RESOLUTION = 1024;
	GLuint shadowMaps[NUMBER_OF_LIGHTS];
	GLuint shadowModelMatrixIndex;
	GLuint shadowViewProjectionMatrixIndex;
	GLuint shadowBiasMatrixIndex;
};

