#version 450

uniform vec4 overrideColor;

uniform ColorBlock {
	vec4 colorA;
	vec4 colorB;
};

smooth in vec4 vertColor;

out vec4 outputColor;

void main()
{
	outputColor = overrideColor + colorA + colorB;
}