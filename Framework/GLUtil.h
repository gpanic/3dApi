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
using namespace std;

struct GLUtil
{
	static GLuint CreateShader(GLenum type, const string &path);
	static GLuint CreateProgram(const vector<GLuint> &shaders);
};
