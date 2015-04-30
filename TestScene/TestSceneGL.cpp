#include "TestSceneGL.h"

GLuint shaderProgram;

GLint modelMatrixIndex;

GLuint materialBindingPoint = 1;
GLuint materialBuffer;

GLuint lightBindingPoint = 2;

GLUtil::Model cube;
glm::mat4 cubeModelMatrix;

GLUtil::Model sphere;
glm::mat4 sphereModelMatrix;

GLUtil::Model sphereSmooth;
glm::mat4 sphereSmoothModelMatrix;

GLUtil::Model monkey;
glm::mat4 monkeyModelMatrix;

GLUtil::Model plane;
glm::mat4 planeModelMatrix;

TestSceneGL::TestSceneGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Scene";
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

	// PREPARE MODELS
	cube = GLUtil::Model("cube.bin", "cube.mtl", true);
	cubeModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, -2.0f));

	sphere = GLUtil::Model("sphere.bin", "sphere.mtl", true);
	sphereModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -2.0f));

	sphereSmooth = GLUtil::Model("sphere_smooth.bin", "sphere_smooth.mtl", true);
	sphereSmoothModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 2.0f));

	monkey = GLUtil::Model("monkey.bin", "monkey.mtl", true);
	monkeyModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f));

	plane = GLUtil::Model("plane.bin", "plane.mtl", true);
	planeModelMatrix = glm::mat4(1.0f);
	planeModelMatrix = glm::scale(planeModelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	planeModelMatrix = glm::translate(planeModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	// PREPARE MATERIAL BUFFER
	materialBuffer;
	glCreateBuffers(1, &materialBuffer);
	glNamedBufferData(materialBuffer, sizeof(Material), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingPoint, materialBuffer);
	GLuint materialBlockIndex = glGetUniformBlockIndex(shaderProgram, "Material");
	glUniformBlockBinding(shaderProgram, materialBlockIndex, materialBindingPoint);

	// UPLOAD LIGHT
	Light light;
	light.position = Vector4(5.0f, 10.0f, 5.0f, 1.0f);
	light.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	light.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	light.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	GLuint lightBuffer;
	glCreateBuffers(1, &lightBuffer);
	glNamedBufferData(lightBuffer, sizeof(Light), &light, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);

	GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Light");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, lightBindingPoint);

	// UPLOAD MVP MATRICES
	modelMatrixIndex = glGetUniformLocation(shaderProgram, "modelMatrix");

	glm::vec3 eye(5.0f, 5.0f, 10.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
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

	glBindVertexArray(cube.vertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));
	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &cube.material);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

	glBindVertexArray(sphere.vertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(sphereModelMatrix));
	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &sphere.material);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, sphere.vertexCount);

	glBindVertexArray(sphereSmooth.vertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(sphereSmoothModelMatrix));
	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &sphereSmooth.material);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, sphereSmooth.vertexCount);

	glBindVertexArray(monkey.vertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(monkeyModelMatrix));
	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &monkey.material);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, monkey.vertexCount);

	glBindVertexArray(plane.vertexArray);
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(planeModelMatrix));
	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &plane.material);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, plane.vertexCount);

}