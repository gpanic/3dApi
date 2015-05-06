#version 450

in vec3 normalViewSpace;
in vec3 positionViewSpace;
in vec3 lightDirViewSpace;

out vec4 outputColor;

uniform vec4 ambient;

uniform Light
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float halfDistance;
} light;

uniform Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	vec3 padding;
}
material;

void main()
{
	vec3 normalViewSpace = normalize(normalViewSpace);
	vec3 viewDirectionViewSpace = normalize(-positionViewSpace);
	vec3 lightDirectionViewSpace = normalize(lightDirViewSpace);
	vec3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);

	float cosIncidence = dot(normalViewSpace, lightDirectionViewSpace);
	cosIncidence = clamp(cosIncidence, 0.0f, 1.0f);

	float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
	phongTerm = clamp(phongTerm, 0.0f, 1.0f);
	phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, material.shininess);

	outputColor = ambient + (material.diffuse * light.diffuse * cosIncidence) + (material.specular * light.specular * phongTerm);
}