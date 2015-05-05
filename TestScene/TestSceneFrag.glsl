#version 450

const int numberOfLights = 2;

in vec3 normalViewSpace;
in vec3 positionViewSpace;
in vec3 lightPositionsViewSpace[numberOfLights];
in vec4 shadowCoord[numberOfLights];

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

layout (binding = 0) uniform sampler2DShadow shadowMap[numberOfLights];

void main()
{
	vec3 normalViewSpace = normalize(normalViewSpace);
	vec3 viewDirectionViewSpace = normalize(-positionViewSpace);

	vec4 ambient = material.ambient * lighting.ambient;
	vec4 diffuse;
	vec4 specular;

	for (int i = 0; i < numberOfLights; ++i)
	{	
		Light light = lighting.lights[i];

		vec3 lightDirectionViewSpace = normalize(lightPositionsViewSpace[i]);

		float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
		cosIncidence = clamp(cosIncidence, 0.0f, 1.0f);

		vec3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);
		float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
		phongTerm = clamp(phongTerm, 0.0f, 1.0f);
		phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
		phongTerm = pow(phongTerm, material.shininess);

		float bias = 0.005 * tan(acos(cosIncidence));
		float visibility = texture(shadowMap[i], vec3(shadowCoord[i].xy, shadowCoord[i].z - bias));
		float diffuseVisibility = mix(1.0f - length(vec3(light.diffuse)) / length(vec3(1.0f)), 1.0f, visibility);

		diffuse += diffuseVisibility * (material.diffuse * light.diffuse * cosIncidence);
		specular += visibility * (material.specular * light.specular * phongTerm);
	}

	outputColor = ambient + diffuse + specular;
}