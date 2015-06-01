#version 450

smooth in vec4 vertColor;

out vec4 outputColor;

uniform ColorBlock
{
	vec4 color;
};

void main()
{
	outputColor = color;
}