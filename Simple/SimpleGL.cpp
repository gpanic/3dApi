#include "SimpleGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;

SimpleGL::SimpleGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Simple";
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

SimpleGL::~SimpleGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool SimpleGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "SimpleVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "SimpleFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(Data::vertices), Data::vertices, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex2));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2().position));

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Data::vertices), Data::vertices, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex().position)));

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	return true;
}


void SimpleGL::Update()
{
}

void SimpleGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glUseProgram(shaderProgram);

	glDrawArrays(GL_TRIANGLES, 0, Data::vertexCount);
}