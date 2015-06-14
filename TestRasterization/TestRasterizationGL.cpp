#include "TestRasterizationGL.h"

TestRasterizationGL::TestRasterizationGL(HINSTANCE hInstance) : GLApp(hInstance)
{
	mAppTitle = "OpenGL Test Rasterization";
	mBenchmarkResultName = "gl_test_rasterization";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

TestRasterizationGL::~TestRasterizationGL()
{
	glDeleteProgram(shaderProgram);
}

bool TestRasterizationGL::InitScene()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	eye = glm::vec3(0.0f, 20.0f, 60.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);

	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_NONE);

	std::vector<GLuint> shaders;
	shaders.push_back(GLUtil::CreateShader(GL_VERTEX_SHADER, shaderPath + "TestRasterizationVert.glsl"));
	shaders.push_back(GLUtil::CreateShader(GL_FRAGMENT_SHADER, shaderPath + "TestRasterizationFrag.glsl"));
	shaderProgram = GLUtil::CreateProgram(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);

	std::string materialName;
	BinaryIO::ReadVertices(modelPath + "sphere_smooth_low_poly.bin", vertices, materialName);
	ObjReader::ReadMtl(modelPath + "sphere_smooth_low_poly.mtl", materialName, material);
	BinaryIO::ReadVector3s(binaryPath + "instance_offsets.bin", offsets);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vertex().position));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint instanceBuffer;
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(Vector3), &offsets[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// UPLOAD LIGHT
	Light light;
	light.position = Vector4(5.0f, 5.0f, 5.0f, 0.0f);
	light.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 ambient(0.1f, 0.1f, 0.1f, 1.0f);

	GLuint lightBuffer;
	glGenBuffers(1, &lightBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, lightBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Light), &light, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);
	GLuint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Light");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, lightBindingPoint);

	GLuint ambientIndex = glGetUniformLocation(shaderProgram, "ambient");
	glProgramUniform4fv(shaderProgram, ambientIndex, 1, (GLfloat *)&ambient);

	// PREPARE MATERIAL
	glGenBuffers(1, &materialBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, materialBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Material), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingPoint, materialBuffer);
	GLuint materialBlockIndex = glGetUniformBlockIndex(shaderProgram, "Material");
	glUniformBlockBinding(shaderProgram, materialBlockIndex, materialBindingPoint);

	// UPLOAD MVP MATRICES
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
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


void TestRasterizationGL::Update()
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

void TestRasterizationGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, materialBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Material), &material);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vertexArray);
	glUseProgram(shaderProgram);
	glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), offsets.size());
}