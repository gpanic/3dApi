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
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vertex().position));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArray;
}