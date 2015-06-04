#include "TestUniformUpdateGL.h"

GLuint shaderProgram;
GLuint vao;
GLuint vbo;

GLuint colorBlockBuffer;

TestUniformUpdateGL::TestUniformUpdateGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Uniform Update";
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

TestUniformUpdateGL::~TestUniformUpdateGL()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool TestUniformUpdateGL::InitScene()
{
	const float vertices[] =
	{
		-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "TestUniformUpdateVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "TestUniformUpdateFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	const float color[]
	{
		0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint colorBlockBindingPoint = 1;

	glGenBuffers(1, &colorBlockBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBlockBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, colorBlockBindingPoint, colorBlockBuffer);

	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "ColorBlock");
	glUniformBlockBinding(shaderProgram, blockIndex, colorBlockBindingPoint);

	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 8 * sizeof(float));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float));

	return true;
}


void TestUniformUpdateGL::Update()
{
	float color2[] = {1.0f, 1.0f, 0.0f, 1.0f};

	for (int i = 0; i < 50000; ++i)
	{
		glNamedBufferSubData(colorBlockBuffer, 0, sizeof(color2), color2);
	}


	//for (int i = 0; i < 50000; ++i)
	//{
	//	GLfloat *pBuffer = reinterpret_cast<GLfloat *>(glMapNamedBufferRange(colorBlockBuffer, 0, sizeof(color2), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
	//	memcpy(pBuffer, color2, sizeof(color2));
	//	glUnmapNamedBuffer(colorBlockBuffer);
	//}
}

void TestUniformUpdateGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glUseProgram(shaderProgram);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}