#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 normalViewSpace;
out vec3 lightDirectionViewSpace;
out vec3 viewDirectionViewSpace;

uniform Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
}
light;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	vec4 positionViewSpace = viewMatrix * modelMatrix * vec4(position, 1.0f);
	gl_Position = projectionMatrix * positionViewSpace;

	normalViewSpace = mat3(viewMatrix) * mat3(modelMatrix) * normal;

	vec3 lightPosViewSpace = vec3(viewMatrix * light.position);
	lightDirectionViewSpace = lightPosViewSpace - vec3(positionViewSpace);

	viewDirectionViewSpace = -vec3(positionViewSpace);
}