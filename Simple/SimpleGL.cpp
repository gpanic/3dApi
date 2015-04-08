#include "SimpleGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;

SimpleGL::SimpleGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Simple";
}

SimpleGL::~SimpleGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool SimpleGL::InitScene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "SimpleVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "SimpleFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	GLfloat vertexData[] = {
		-0.75f, -0.75f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.75f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.75f, -0.75f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}


void SimpleGL::Update()
{
}

void SimpleGL::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));


	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}