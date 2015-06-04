#version 450

out vec4 outputColor;

uniform ColorBlock
{
	vec4 color;
};

void main()
{
	outputColor = color;
}