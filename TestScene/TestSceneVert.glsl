#version 450

const int numberOfLights = 2;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 normalViewSpace;
out vec3 positionViewSpace;
out vec3 lightPositionsViewSpace[numberOfLights];
out vec4 shadowCoord[numberOfLights];

struct Light
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float halfDistance;
};

uniform Lighting
{
	vec4 ambient;
	Light lights[numberOfLights];
}
lighting;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 shadowViewProjectionMatrix[numberOfLights];
uniform mat4 shadowBiasMatrix;

void main()
{
	vec4 posViewSpace = viewMatrix * modelMatrix * vec4(position, 1.0f);
	positionViewSpace = vec3(posViewSpace);
	normalViewSpace = mat3(viewMatrix) * mat3(modelMatrix) * normal;

	for (int i = 0; i < numberOfLights; ++i)
	{
		lightPositionsViewSpace[i] = vec3(viewMatrix * lighting.lights[i].position);
		shadowCoord[i] = shadowBiasMatrix * shadowViewProjectionMatrix[i] * modelMatrix * vec4(position, 1.0f);
	}

	gl_Position = projectionMatrix * posViewSpace;
}