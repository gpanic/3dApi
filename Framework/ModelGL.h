#pragma once
#include "GLUtil.h"
#include "Material.h"
#include "Vertex.h"
#include "ObjReader.h"
#include "BinaryIO.h"

class ModelGL
{
public:
	ModelGL();
	ModelGL(const std::string &model, const std::string &mtlPath, const glm::mat4 &matrix, bool binary = false);

	void LoadModel(const std::string &model, const std::string &mtlPath, const glm::mat4 &matrix, bool binary = false);

	GLuint vertexArray;
	unsigned long long vertexCount;
	Material material;
	GLuint shaderProgram;
	glm::mat4 modelMatrix;
private:
	static GLuint CreateVertexArray(const std::vector<Vertex> &vertices);
	static GLuint CreateMaterialBuffer(const Material &material);
};

