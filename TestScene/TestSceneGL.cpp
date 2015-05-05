#include "TestSceneGL.h"

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

	// PREPARE LIGHTING
	lighting.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	Light light1;
	light1.position = Vector4(-5.0f, 5.0f, 5.0f, 0.0f);
	light1.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light1.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	Light light2;
	light2.position = Vector4(5.0f, 5.0f, 5.0f, 0.0f);
	light2.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light2.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	lighting.lights[0] = light1;
	lighting.lights[1] = light2;

	GLuint lightBuffer;
	glCreateBuffers(1, &lightBuffer);
	glNamedBufferData(lightBuffer, sizeof(Lighting), &lighting, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);
	GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Lighting");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, lightBindingPoint);

	RenderShadowMaps();

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

	for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
	}

	for (ModelGL model : models)
	{
		glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
		glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
		glBindVertexArray(model.vertexArray);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
	}
}

void TestSceneGL::RenderShadowMaps()
{
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glm::mat4 depthViewProjectionMatrices[NUMBER_OF_LIGHTS];
	for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)
	{
		glGenTextures(1, &shadowMaps[i]);
		glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_RESOLUTION, SHADOW_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMaps[i], 0);

		glDrawBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return;

		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glm::vec3 lightDir = glm::normalize(glm::vec3(lighting.lights[i].position.x, lighting.lights[i].position.y, lighting.lights[i].position.z));
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		depthViewProjectionMatrices[i] = depthProjectionMatrix * depthViewMatrix;

		shadowModelMatrixIndex = glGetUniformLocation(shadowShaderProgram, "modelMatrix");
		shadowViewProjectionMatrixIndex = glGetUniformLocation(shadowShaderProgram, "viewProjectionMatrix");

		glProgramUniformMatrix4fv(shadowShaderProgram, shadowViewProjectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(depthViewProjectionMatrices[i]));

		for (ModelGL model : models)
		{
			glProgramUniformMatrix4fv(shadowShaderProgram, shadowModelMatrixIndex, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
			glNamedBufferSubData(materialBuffer, 0, sizeof(Material), &model.material);
			glBindVertexArray(model.vertexArray);
			glUseProgram(shadowShaderProgram);
			glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	shadowViewProjectionMatrixIndex = glGetUniformLocation(shaderProgram, "shadowViewProjectionMatrix");
	shadowBiasMatrixIndex = glGetUniformLocation(shaderProgram, "shadowBiasMatrix");
	glProgramUniformMatrix4fv(shaderProgram, shadowViewProjectionMatrixIndex, 2, GL_FALSE, glm::value_ptr(depthViewProjectionMatrices[0]));
	glProgramUniformMatrix4fv(shaderProgram, shadowBiasMatrixIndex, 1, GL_FALSE, glm::value_ptr(biasMatrix));
}