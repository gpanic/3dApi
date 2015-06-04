#version 450

in vec4 origPos;

out vec4 outputColor;

void main()
{
	outputColor = vec4((origPos.x + 1.0f) / 2.0f,
						(origPos.y + 1.0f) / 2.0f,
						(origPos.z + 1.0f) / 2.0f,
						1.0f);
}