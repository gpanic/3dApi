#version 450

layout(location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position =  viewProjectionMatrix * modelMatrix * position;
}

