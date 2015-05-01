#include "TestSceneGL.h"

GLuint shaderProgram;

GLuint materialBindingPoint = 1;
GLuint lightBindingPoint = 2;
GLuint materialBuffer;

GLuint modelMatrixIndex;
GLuint viewMatrixIndex;
GLuint projectionMatrixIndex;

std::vector<ModelGL> models;

const float rotDelta = 10.0f;
const glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(5.0f, 5.0f, 8.0f);
glm::vec3 right(1.0f, 0.0f, 0.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);

TestSceneGL::TestSceneGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Scene";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
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
	glm::mat4 matrix;

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, -2.0f));
	models.push_back(ModelGL("torus.bin", "torus.mtl", shaderProgram, matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -2.0f));
	models.push_back(ModelGL("sphere.bin", "sphere.mtl", shaderProgram, matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 2.0f));
	models.push_back(ModelGL("sphere_smooth.bin", "sphere_smooth.mtl", shaderProgram, matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f));
	models.push_back(ModelGL("monkey.bin", "monkey.mtl", shaderProgram, matrix, true));

	matrix = glm::mat4(1.0f);
	matrix = glm::scale(matrix, glm::vec3(5.0f, 5.0f, 5.0f));
	matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
	models.push_back(ModelGL("plane.bin", "plane.mtl", shaderProgram, matrix, true));

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
	light.diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	light.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	GLuint lightBuffer;
	glCreateBuffers(1, &lightBuffer);
	glNamedBufferData(lightBuffer, sizeof(Light), &light, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);

	GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Light");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, lightBindingPoint);

	// UPLOAD MVP MATRICES
	modelMatrixIndex = glGetUniformLocation(shaderProgram, "modelMatrix");
	viewMatrixIndex = glGetUniformLocation(shaderProgram, "viewMatrix");
	projectionMatrixIndex = glGetUniformLocation(shaderProgram, "projectionMatrix");

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 1.0f, 500.0f);
	
	glProgramUniformMatrix4fv(shaderProgram, projectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void TestSceneGL::Update()
{
	float rotAmount = 0.0f;
	glm::mat4 rotMatrix;
	if (input.right || input.left)
	{
		if (input.right)
			rotAmount = rotDelta;
		if (input.left)
			rotAmount = -rotDelta;
		rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotAmount), up);
	}
	else if (input.up || input.down)
	{
		if (input.up)
			rotAmount = -rotDelta;
		if (input.down)
			rotAmount = rotDelta;
		rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotAmount), right);
	}

	eye = glm::vec3(rotMatrix * glm::vec4(eye, 1.0f));
	right = glm::normalize(-glm::cross(up, (center - eye)));

	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
	glProgramUniformMatrix4fv(shaderProgram, viewMatrixIndex, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void TestSceneGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (ModelGL model : models)
	{
		glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
		glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
		glBindVertexArray(model.vertexArray);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
	}
}