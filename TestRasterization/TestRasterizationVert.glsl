#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 offset;

out vec3 normalViewSpace;
out vec3 positionViewSpace;
out vec3 lightDirViewSpace;

uniform Light
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float halfDistance;
} light;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	vec4 posViewSpace = viewMatrix * (modelMatrix * position + vec4(offset, 0.0f));
	positionViewSpace = vec3(posViewSpace);
	normalViewSpace = mat3(viewMatrix) * mat3(modelMatrix) * normal;

	lightDirViewSpace = vec3(viewMatrix * light.position);
	
	gl_Position = projectionMatrix * posViewSpace;
}