#version 450

layout(location = 0) in vec4 position;

out vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	vec3 color1 = mix(vec3(1,0,0), vec3(0,1,0), ((position.x + 10.0f) / 20.0f));
	vec3 color2 = mix(vec3(0,0,1), vec3(1,1,1), ((position.x + 10.0f) / 20.0f));
	color = mix(color1, color2, ((position.z + 10.0f) / 20.0f));
}