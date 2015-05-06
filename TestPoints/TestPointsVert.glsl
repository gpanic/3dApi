#version 450

layout(location = 0) in vec4 position;

out vec4 origPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	origPos = position;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}