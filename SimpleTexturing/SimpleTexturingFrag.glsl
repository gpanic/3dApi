#version 450

smooth in vec2 uv;

out vec4 outputColor;

uniform sampler2D textureSampler;

void main()
{
	outputColor = texture2D(textureSampler, uv);
}