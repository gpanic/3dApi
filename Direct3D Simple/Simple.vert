#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 vertColor;

void main()
{
	vertColor = color;
	gl_Position = position;
}