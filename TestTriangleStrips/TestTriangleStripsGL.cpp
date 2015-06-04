#include "TestTriangleStripsGL.h"

TestTriangleStripsGL::TestTriangleStripsGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Triangle Strips";
	mBenchmarkResultName = "gl_test_triangle_strips";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

TestTriangleStripsGL::~TestTriangleStripsGL()
{
	glDeleteProgram(shaderProgram);
}

bool TestTriangleStripsGL::InitScene()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	eye = glm::vec3(0.0f, 18.0f, 18.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_NONE);
	//glPolygonMode(GL_FRONT, GL_LINE);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, shaderPath + "TestTriangleStripsVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, shaderPath + "TestTriangleStripsFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	BinaryIO::ReadVector4s(binaryPath + "triangle_strip_plane.bin", vertices);

	glCreateVertexArrays(1, &vertexArray);

	GLuint vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferData(vertexBuffer, vertices.size() * sizeof(Vector4), &vertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vertexArray, 0, 0);
	glVertexArrayVertexBuffer(vertexArray, 0, vertexBuffer, 0, sizeof(Vector4));
	glEnableVertexArrayAttrib(vertexArray, 0);
	glVertexArrayAttribFormat(vertexArray, 0, 4, GL_FLOAT, GL_FALSE, 0);

	// UPLOAD MVP MATRICES
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 1.0f, 500.0f);

	modelMatrixIndex = glGetUniformLocation(shaderProgram, "modelMatrix");
	viewMatrixIndex = glGetUniformLocation(shaderProgram, "viewMatrix");
	projectionMatrixIndex = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glProgramUniformMatrix4fv(shaderProgram, modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glProgramUniformMatrix4fv(shaderProgram, viewMatrixIndex, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glProgramUniformMatrix4fv(shaderProgram, projectionMatrixIndex, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	return true;
}


void TestTriangleStripsGL::Update()
{
	if (processInput)
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
}

void TestTriangleStripsGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArray);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}