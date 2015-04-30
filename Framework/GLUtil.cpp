#include "GLUtil.h"

GLuint GLUtil::CreateShader(GLenum type, const std::string &path)
{
	std::string shaderStr = Util::ReadFile(path);
	const char* shaderChar = shaderStr.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderChar, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string infoLog(infoLogLength, ' ');
		glGetShaderInfoLog(shader, infoLogLength, NULL, &infoLog[0]);

		std::string shaderType;
		switch (type)
		{
		case GL_VERTEX_SHADER: shaderType = "vertex"; break;
		case GL_FRAGMENT_SHADER: shaderType = "fragment"; break;
		}

		infoLog.insert(0, "Compile error in " + shaderType + " shader: ");
		infoLog.append("\n");
		OutputDebugString(infoLog.c_str());
	}

	return shader;
}

GLuint GLUtil::CreateProgram(const std::vector<GLuint> &shaders)
{
	GLuint program = glCreateProgram();

	for (GLuint shader : shaders)
		glAttachShader(program, shader);

	glLinkProgram(program);

	GLint status;
	glGetShaderiv(program, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		std::string infoLog(infoLogLength, ' ');
		glGetProgramInfoLog(program, infoLogLength, NULL, &infoLog[0]);

		infoLog.insert(0, "Liner error: ");
		infoLog.append("\n");
		OutputDebugString(infoLog.c_str());
	}

	for (GLuint shader : shaders)
		glDetachShader(program, shader);

	return program;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint GLUtil::LoadDDS(const std::string &imagepath)
{

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath.c_str(), "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glTextureStorage2D(textureID, mipMapCount, format, width, height);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		//glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
		//	0, size, buffer + offset);
		glCompressedTextureSubImage2D(textureID, level, 0, 0, width, height, format, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;
}

GLUtil::Model::Model() : vertexArray(0), vertexCount(0)
{
}

GLUtil::Model::Model(const std::string &objPath, const std::string &mtlPath, bool binary)
{
	std::vector<Vertex> vertices;
	if (!binary)
	{
		ObjReader::Read(objPath, mtlPath, vertices, material);
	}
	else
	{
		std::string materialName;
		BinaryIO::ReadVertices(objPath, vertices, materialName);
		ObjReader::ReadMtl(mtlPath, materialName, material);
	}
	vertexArray = CreateVertexArray(vertices);
	vertexCount = vertices.size();
}

GLuint GLUtil::Model::CreateVertexArray(const std::vector<Vertex> &vertices)
{
	GLuint vertexArray;
	glCreateVertexArrays(1, &vertexArray);

	GLuint vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferData(vertexBuffer, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexArrayAttribBinding(vertexArray, 0, 0);
	glVertexArrayAttribBinding(vertexArray, 1, 0);
	glVertexArrayVertexBuffer(vertexArray, 0, vertexBuffer, 0, sizeof(Vertex));

	glEnableVertexArrayAttrib(vertexArray, 0);
	glEnableVertexArrayAttrib(vertexArray, 1);
	glVertexArrayAttribFormat(vertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex().position));
	return vertexArray;
}