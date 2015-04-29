#include "TestSceneGL.h"

GLuint shaderProgram;

GLint modelMatrixIndex;

GLuint cubeVertexArray;
GLuint cubeVertexBuffer;
std::vector<Vertex> cubeVertices;
glm::mat4 cubeModelMatrix;

GLuint sphereVertexArray;
GLuint sphereVertexBuffer;
std::vector<Vertex> sphereVertices;
glm::mat4 sphereModelMatrix;

GLuint sphereSmoothVertexArray;
GLuint sphereSmoothVertexBuffer;
std::vector<Vertex> sphereSmoothVertices;
glm::mat4 sphereSmoothModelMatrix;

TestSceneGL::TestSceneGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Scene";

	//bgColor = Color(0.0f, 0.2f, 0.4f, 1.0f);
}

TestSceneGL::~TestSceneGL()
{
	glDeleteProgram(shaderProgram);
}

bool TestSceneGL::InitScene()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	// COMPILE SHADERS
	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "TestSceneVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "TestSceneFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);


	// LOAD CUBE
	ObjReader::Read("cube.obj", cubeVertices);

	glCreateVertexArrays(1, &cubeVertexArray);

	glCreateBuffers(1, &cubeVertexBuffer);
	glNamedBufferData(cubeVertexBuffer, cubeVertices.size() * sizeof(Vertex), &cubeVertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(cubeVertexArray, 0, 0);
	glVertexArrayAttribBinding(cubeVertexArray, 1, 0);
	glVertexArrayVertexBuffer(cubeVertexArray, 0, cubeVertexBuffer, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(cubeVertexArray, 0);
	glEnableVertexArrayAttrib(cubeVertexArray, 1);
	glVertexArrayAttribFormat(cubeVertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(cubeVertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3));

	cubeModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, -8.0f));

	// LOAD SPHERE
	ObjReader::Read("sphere.obj", sphereVertices);

	glCreateVertexArrays(1, &sphereVertexArray);

	glCreateBuffers(1, &sphereVertexBuffer);
	glNamedBufferData(sphereVertexBuffer, sphereVertices.size() * sizeof(Vertex), &sphereVertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(sphereVertexArray, 0, 0);
	glVertexArrayAttribBinding(sphereVertexArray, 1, 0);
	glVertexArrayVertexBuffer(sphereVertexArray, 0, sphereVertexBuffer, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(sphereVertexArray, 0);
	glEnableVertexArrayAttrib(sphereVertexArray, 1);
	glVertexArrayAttribFormat(sphereVertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(sphereVertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3));

	sphereModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec3(-2.0f, -2.0f, -8.0f)));

	// LOAD SMOOTH SPHERE
	ObjReader::Read("sphere_smooth.obj", sphereSmoothVertices);

	glCreateVertexArrays(1, &sphereSmoothVertexArray);

	glCreateBuffers(1, &sphereSmoothVertexBuffer);
	glNamedBufferData(sphereSmoothVertexBuffer, sphereSmoothVertices.size() * sizeof(Vertex), &sphereSmoothVertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(sphereSmoothVertexArray, 0, 0);
	glVertexArrayAttribBinding(sphereSmoothVertexArray, 1, 0);
	glVertexArrayVertexBuffer(sphereSmoothVertexArray, 0, sphereSmoothVertexBuffer, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(sphereSmoothVertexArray, 0);
	glEnableVertexArrayAttrib(sphereSmoothVertexArray, 1);
	glVertexArrayAttribFormat(sphereSmoothVertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(sphereSmoothVertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3));

	sphereSmoothModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec3(-2.0f, 2.0f, -8.0f)));

	
	modelMatrixIndex = glGetUniformLocation(shaderProgram, "modelMatrix");

	glm::vec3 eye(0.0f, 0.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, -8.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
	GLint viewMatrixIndex = glGetUniformLocation(shaderProgram, "viewMatrix");
	glProgramUniformMatrix4fv(shaderProgram, viewMatrixIndex, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 1.0f, 500.0f);
	GLint projectionMatrixIndex = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glProgramUniformMatrix4fv(shaderProgram, projectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}


void TestSceneGL::Update()
{
}

void TestSceneGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(cubeVertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

	glBindVertexArray(sphereVertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(sphereModelMatrix));
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());

	glBindVertexArray(sphereSmoothVertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(sphereSmoothModelMatrix));
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, sphereSmoothVertices.size());

}