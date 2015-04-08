#include "GLUtil.h"

GLuint GLUtil::CreateShader(GLenum type, const string &path)
{
	string shaderStr = Util::ReadFile(path);
	const char* shaderChar = shaderStr.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint GLUtil::CreateProgram(const vector<GLuint> &shaders)
{
	GLuint program = glCreateProgram();

	for (GLuint shader : shaders)
		glAttachShader(program, shader);

	glLinkProgram(program);

	for (GLuint shader : shaders)
		glDetachShader(program, shader);

	return program;
}