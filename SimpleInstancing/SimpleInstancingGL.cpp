#include "SimpleInstancingGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;
GLuint ivbo;
GLuint ibo;

SimpleInstancingGL::SimpleInstancingGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Simple Instancing";
}

SimpleInstancingGL::~SimpleInstancingGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ivbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}

bool SimpleInstancingGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "SimpleInstancingVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "SimpleInstancingFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(Data::vertices), Data::vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &ivbo);
	glNamedBufferData(ivbo, sizeof(Data::instanceData), Data::instanceData, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 1);
	glVertexArrayAttribBinding(vao, 2, 1);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 4 * sizeof(float));
	glVertexArrayVertexBuffer(vao, 1, ivbo, 0, 2 * 4 * sizeof(float));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayBindingDivisor(vao, 0, 0);
	glVertexArrayBindingDivisor(vao, 1, 1);
	glVertexArrayBindingDivisor(vao, 2, 1);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float));

	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, sizeof(Data::indices), Data::indices, GL_STATIC_DRAW);

	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Data::vertices), Data::vertices, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//glVertexAttribDivisor(0, 0);

	//glGenBuffers(1, &ivbo);
	//glBindBuffer(GL_ARRAY_BUFFER, ivbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Data::instanceData), Data::instanceData, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(float), 0);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 2 * 4 * sizeof(float), (void*)(4 * sizeof(float)));
	//glVertexAttribDivisor(1, 1);
	//glVertexAttribDivisor(2, 1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Data::indices), Data::indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	return true;
}


void SimpleInstancingGL::Update()
{
}

void SimpleInstancingGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(shaderProgram);
	glDrawElementsInstanced(GL_TRIANGLES, Data::indexCount, GL_UNSIGNED_SHORT, 0, Data::instanceCount);
}