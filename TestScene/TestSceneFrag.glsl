#version 450

in vec3 normalViewSpace;
in vec3 lightDirectionViewSpace;
in vec3 viewDirectionViewSpace;

out vec4 outputColor;

uniform vec4 ambientMaterial = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 1.0f);
uniform vec4 diffuseMaterial = vec4(0.6f, 0.6f, 0.6f, 1.0f);
uniform vec4 diffuseLight = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 specularMaterial = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 specularLight = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float shininessFactor = 128.0f;

void main()
{
	vec3 normalViewSpace = normalize(normalViewSpace);
	vec3 lightDirectionViewSpace = normalize(lightDirectionViewSpace);
	vec3 viewDirectionViewSpace = normalize(viewDirectionViewSpace);

	vec3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);

	float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
	cosIncidence = clamp(cosIncidence, 0.0f, 1.0f);

	float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
	phongTerm = clamp(phongTerm, 0.0f, 1.0f);
	phongTerm = pow(phongTerm, shininessFactor);

	outputColor = (ambientMaterial * ambientLight) + (diffuseMaterial * diffuseLight * cosIncidence) + (specularMaterial * specularLight * phongTerm);
}