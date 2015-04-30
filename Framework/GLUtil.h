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
#include "ObjReader.h"
#include "BinaryIO.h"

namespace GLUtil
{
	GLuint CreateShader(GLenum type, const std::string &path);
	GLuint CreateProgram(const std::vector<GLuint> &shaders);
	GLuint LoadDDS(const std::string &imagepath);

	struct Model
	{
		Model();
		Model(const std::string &model, bool binary = false);

		GLuint vertexArray;
		unsigned long long vertexCount;
	private:
		static GLuint CreateVertexArray(const std::vector<Vertex> &vertices);
	};
};
