#include "SimpleTexturingGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;
GLuint ibo;
GLuint texture;
GLuint sampler;

SimpleTexturingGL::SimpleTexturingGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Simple Texturing";
}

SimpleTexturingGL::~SimpleTexturingGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool SimpleTexturingGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "SimpleTexturingVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "SimpleTexturingFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(Data::verticesDX), Data::verticesDX, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex().position));

	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, sizeof(Data::indices), Data::indices, GL_STATIC_DRAW);

	texture = GLUtil::LoadDDS("uv_grid.dds");

	sampler = glGetUniformLocation(shaderProgram, "textureSampler");

	glBindTexture(GL_TEXTURE_2D, 0);

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


void SimpleTexturingGL::Update()
{
}

void SimpleTexturingGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(sampler, 0);

	glDrawElements(GL_TRIANGLES, Data::indexCount, GL_UNSIGNED_SHORT, 0);
}