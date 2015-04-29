#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 normalViewSpace;
out vec3 lightDirectionViewSpace;
out vec3 viewDirectionViewSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPositionViewSpace = vec3(100.0f, 100.0f, 100.0f);

void main()
{
	vec4 positionViewSpace = viewMatrix * modelMatrix * vec4(position, 1.0f);
	gl_Position = projectionMatrix * positionViewSpace;

	normalViewSpace = mat3(viewMatrix) * mat3(modelMatrix) * normal;
	lightDirectionViewSpace = lightPositionViewSpace - vec3(positionViewSpace);
	viewDirectionViewSpace = -vec3(positionViewSpace);
	
	//vec3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);

	//float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
	//cosIncidence = clamp(cosIncidence, 0.0f, 1.0f);

	//float phongTerm = dot(viewDirection, reflectDirectionViewSpace);
	//phongTerm = clamp(phongTerm, 0.0f, 1.0f);
	//phongTerm = pow(phongTerm, shininessFactor);

	//vertexColor = (ambientMaterial * ambientLight) + (diffuseMaterial * diffuseLight * cosIncidence) + (specularMaterial * specularLight * phongTerm);
}