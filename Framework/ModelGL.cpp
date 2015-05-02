#include "ModelGL.h"

ModelGL::ModelGL() : vertexArray(0), vertexCount(0), shaderProgram(0)
{
}

ModelGL::ModelGL(const std::string &objPath, const std::string &mtlPath, const glm::mat4 &matrix, bool binary)
{
	LoadModel(objPath, mtlPath, matrix, binary);
}

void ModelGL::LoadModel(const std::string &objPath, const std::string &mtlPath, const glm::mat4 &matrix, bool binary)
{
	std::vector<Vertex> vertices;
	if (!binary)
	{
		ObjReader::Read(objPath, mtlPath, vertices, material);
	}
	else
	{
		std::string materialName;
		BinaryIO::ReadVertices(objPath, vertices, materialName);
		ObjReader::ReadMtl(mtlPath, materialName, material);
	}

	vertexArray = CreateVertexArray(vertices);
	vertexCount = vertices.size();
	modelMatrix = glm::mat4(matrix);
}

GLuint ModelGL::CreateVertexArray(const std::vector<Vertex> &vertices)
{
	GLuint vertexArray;
	glCreateVertexArrays(1, &vertexArray);

	GLuint vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferData(vertexBuffer, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vertexArray, 0, 0);
	glVertexArrayAttribBinding(vertexArray, 1, 0);
	glVertexArrayVertexBuffer(vertexArray, 0, vertexBuffer, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(vertexArray, 0);
	glEnableVertexArrayAttrib(vertexArray, 1);
	glVertexArrayAttribFormat(vertexArray, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex().position));
	return vertexArray;
}