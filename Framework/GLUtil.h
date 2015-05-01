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
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace GLUtil
{
	GLuint CreateShader(GLenum type, const std::string &path);
	GLuint CreateProgram(const std::vector<GLuint> &shaders);
	GLuint LoadDDS(const std::string &imagepath);
};
