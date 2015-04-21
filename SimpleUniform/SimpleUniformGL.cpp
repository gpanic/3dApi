#include "SimpleUniformGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;

SimpleUniformGL::SimpleUniformGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Simple Uniform";
}

SimpleUniformGL::~SimpleUniformGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool SimpleUniformGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "SimpleUniformVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "SimpleUniformFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);
	glUseProgram(shaderProgram);

	GLint overrideColor = glGetUniformLocation(shaderProgram, "overrideColor");
	glProgramUniform4f(shaderProgram, overrideColor, 1.0f, 0.0f, 0.0f, 1.0f);

	GLuint bindingPoint = 1;

	float colorsBlock[8] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0f };
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorsBlock), colorsBlock, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);

	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "ColorBlock");
	glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(Data::vertices), Data::vertices, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex().position));

	return true;
}


void SimpleUniformGL::Update()
{
}

void SimpleUniformGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, Data::vertexCount);
}