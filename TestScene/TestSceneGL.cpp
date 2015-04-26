#include "TestSceneGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;
GLuint ibo;

std::vector<Vertex> vertices;
std::vector<Face> faces;

TestSceneGL::TestSceneGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Scene";
}

TestSceneGL::~TestSceneGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool TestSceneGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "TestSceneVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "TestSceneFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	ObjReader::Read("cube.obj", vertices, faces);

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);

	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, faces.size() * sizeof(Face), &faces[0], GL_STATIC_DRAW);

	return true;
}


void TestSceneGL::Update()
{
}

void TestSceneGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(shaderProgram);

	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_SHORT, 0);
}