#version 450

const int numberOfLights = 2;

in vec3 normalViewSpace;
in vec3 positionViewSpace;
in vec3 lightPositionsViewSpace[numberOfLights];
in vec4 shadowCoord;

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

layout (binding = 0) uniform sampler2D shadowMap;


void main()
{
	vec3 normalViewSpace = normalize(normalViewSpace);

	vec4 ambient = material.ambient * lighting.ambient;
	vec4 diffuse;
	vec4 specular;

	float cosTheta;
	for (int i = 1; i < numberOfLights; ++i)
	{	
		Light light = lighting.lights[i];

		vec3 lightDirectionViewSpace = light.position.w == 0.0f ? vec3(lightPositionsViewSpace[i]) : vec3(lightPositionsViewSpace[i] - positionViewSpace);
		lightDirectionViewSpace = normalize(lightDirectionViewSpace);
		vec3 viewDirectionViewSpace = normalize(-positionViewSpace);


		float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
		cosIncidence = clamp(cosIncidence, 0.0f, 1.0f);
		cosTheta = cosIncidence;

		vec3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);
		float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
		phongTerm = clamp(phongTerm, 0.0f, 1.0f);
		phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
		phongTerm = pow(phongTerm, material.shininess);

		vec3 difference = lightPositionsViewSpace[i] - positionViewSpace;

		float attenuation = 1.0f;
		if (light.position.w != 0.0f)
			attenuation = 1.0f / (1.0f + (1.0f / (light.halfDistance * light.halfDistance)) * length(difference));
		
		diffuse += attenuation * (material.diffuse * light.diffuse * cosIncidence);
		specular += attenuation * (material.specular * light.specular * phongTerm);
	}

	float visibility = 1.0f;
	if (texture(shadowMap, shadowCoord.xy).z < shadowCoord.z)
	{
		visibility = 0.0f;
	}

	//float bias = 0.005*tan(acos(cosTheta));
	//float visibility = texture(shadowMap, vec3(shadowCoord.xy, (shadowCoord.z - bias) / shadowCoord.w));
	outputColor = ambient + visibility * diffuse + visibility * specular;
}