#version 450

in vec3 normalViewSpace;
in vec3 lightDirectionViewSpace;
in vec3 viewDirectionViewSpace;

out vec4 outputColor;

uniform Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	vec3 padding;
}
material;

uniform Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
}
light;

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
	phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, material.shininess);

	outputColor = (material.ambient * light.ambient) + (material.diffuse * light.diffuse * cosIncidence) + (material.specular * light.specular * phongTerm);
}