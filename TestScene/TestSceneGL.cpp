#include "TestSceneGL.h"

GLuint shaderProgram;
GLuint shadowShaderProgram;

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

// SHADOW MAPPING
const int SHADOW_RESOLUTION = 1024;
GLuint depthTexture;
GLuint depthModelMatrixIndex;
GLuint depthViewProjectionMatrixIndex;
GLuint depthBiasMatrixIndex;


TestSceneGL::TestSceneGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Scene";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

TestSceneGL::~TestSceneGL()
{
	glDeleteProgram(shaderProgram);
}

bool TestSceneGL::InitScene()
{
	// COMPILE SHADERS
	std::vector<GLuint> shadowShaders;
	shadowShaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "ShadowMappingVert.glsl"));
	shadowShaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "ShadowMappingFrag.glsl"));
	shadowShaderProgram = GLUtil::CreateProgram(shadowShaders);
	for_each(shadowShaders.begin(), shadowShaders.end(), glDeleteShader);
	shadowShaders.clear();

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, "TestSceneVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, "TestSceneFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);
	shaders.clear();

	// PREPARE MODELS
	glm::mat4 matrix;

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, -2.0f));
	models.push_back(ModelGL("torus.bin", "torus.mtl", matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -2.0f));
	models.push_back(ModelGL("sphere.bin", "sphere.mtl", matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 2.0f));
	models.push_back(ModelGL("sphere_smooth.bin", "sphere_smooth.mtl", matrix, true));

	matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 2.0f));
	models.push_back(ModelGL("monkey.bin", "monkey.mtl", matrix, true));

	matrix = glm::mat4(1.0f);
	matrix = glm::scale(matrix, glm::vec3(5.0f, 5.0f, 5.0f));
	matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 0.0f));
	models.push_back(ModelGL("plane.bin", "plane.mtl", matrix, true));

	// SHADOW MAPPING
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

	glm::vec3 lightInvDir = glm::normalize(glm::vec3(5.0f, 5.0f, 5.0f));
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	depthModelMatrixIndex = glGetUniformLocation(shadowShaderProgram, "modelMatrix");
	depthViewProjectionMatrixIndex = glGetUniformLocation(shadowShaderProgram, "viewProjectionMatrix");

	glProgramUniformMatrix4fv(shadowShaderProgram, depthViewProjectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(depthViewProjectionMatrix));

	for (ModelGL model : models)
	{
		glProgramUniformMatrix4fv(shadowShaderProgram, depthModelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
		glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
		glBindVertexArray(model.vertexArray);
		glUseProgram(shadowShaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthViewProjectionMatrixIndex = glGetUniformLocation(shaderProgram, "depthViewProjectionMatrix");
	depthBiasMatrixIndex = glGetUniformLocation(shaderProgram, "depthBiasMatrix");
	glProgramUniformMatrix4fv(shaderProgram, depthViewProjectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(depthViewProjectionMatrix));
	glProgramUniformMatrix4fv(shaderProgram, depthBiasMatrixIndex, 1, GL_FALSE, glm::value_ptr(biasMatrix));

	// SETUP FRAMEBUFFER
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glViewport(0, 0, mWidth, mHeight);

	// PREPARE MATERIAL BUFFER
	materialBuffer;
	glCreateBuffers(1, &materialBuffer);
	glNamedBufferData(materialBuffer, sizeof(Material), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingPoint, materialBuffer);
	GLuint materialBlockIndex = glGetUniformBlockIndex(shaderProgram, "Material");
	glUniformBlockBinding(shaderProgram, materialBlockIndex, materialBindingPoint);

	// UPLOAD LIGHTING
	Lighting lighting;
	lighting.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	Light point;
	point.position = Vector4(0.0f, 5.0f, 0.0f, 1.0f);
	point.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	point.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	point.halfDistance = 2.0f;

	Light directional;
	directional.position = Vector4(5.0f, 5.0f, 5.0f, 1.0f);
	directional.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	directional.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	lighting.lights[0] = point;
	lighting.lights[1] = directional;

	GLuint lightBuffer;
	glCreateBuffers(1, &lightBuffer);
	glNamedBufferData(lightBuffer, sizeof(Lighting), &lighting, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);
	GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Lighting");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, lightBindingPoint);

	// UPLOAD MVP MATRICES
	modelMatrixIndex = glGetUniformLocation(shaderProgram, "modelMatrix");
	viewMatrixIndex = glGetUniformLocation(shaderProgram, "viewMatrix");
	projectionMatrixIndex = glGetUniformLocation(shaderProgram, "projectionMatrix");

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 1.0f, 500.0f);
	glProgramUniformMatrix4fv(shaderProgram, projectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	for (ModelGL model : models)
	{
		glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
		glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
		glBindVertexArray(model.vertexArray);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
	}
	//for (ModelGL model : models)
	//{
	//	glProgramUniformMatrix4fv(shadowShaderProgram, depthModelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
	//	glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
	//	glBindVertexArray(model.vertexArray);
	//	glUseProgram(shadowShaderProgram);
	//	glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
	//}
}