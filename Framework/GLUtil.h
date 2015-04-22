#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glext.h>
#include <wglext.h>
#include "Util.h"

struct GLUtil
{
	static GLuint CreateShader(GLenum type, const std::string &path);
	static GLuint CreateProgram(const std::vector<GLuint> &shaders);
	static GLuint LoadDDS(const std::string &iamagepath);
};
