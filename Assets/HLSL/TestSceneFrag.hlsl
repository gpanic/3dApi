static const int numberOfLights = 2;

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 positionViewSpace : TEXCOORD1;
	float3 lightPositionsViewSpace[numberOfLights] : TEXCOORD2;
	float4 shadowCoord[numberOfLights] : TEXCOORD4;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shininess;
	float3 padding;
};

struct Light
{
	float4 position;
	float4 diffuse;
	float4 specular;
	float halfDistance;
};

struct Lighting
{
	float4 ambient;
	Light lights[numberOfLights];
};

cbuffer Lighting : register(b3)
{
	Lighting lighting;
};

cbuffer Material : register(b0)
{
	Material material;
};

Texture2D shadowMap[numberOfLights] : register(t0);
SamplerComparisonState shadowMapSampler : register(s0);

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	float3 normalViewSpace = normalize(input.normalViewSpace);
	float3 viewDirectionViewSpace = normalize(-input.positionViewSpace);

	float4 ambient = material.ambient * lighting.ambient;
	float4 diffuse = 0.0f;
	float4 specular = 0.0f;

	for (int i = 0; i < numberOfLights; ++i)
	{
		Light light = lighting.lights[i];

		float3 lightDirectionViewSpace = normalize(input.lightPositionsViewSpace[i]);

		float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
		cosIncidence = saturate(cosIncidence);

		float3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);
		float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
		phongTerm = saturate(phongTerm);
		phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
		phongTerm = pow(phongTerm, material.shininess);

		input.shadowCoord[i].y = 1.0f - input.shadowCoord[i].y;

		float bias = 0.005 * tan(acos(cosIncidence));
		float visibility = shadowMap[i].SampleCmpLevelZero(shadowMapSampler, input.shadowCoord[i].xy, input.shadowCoord[i].z - bias);
		float diffuseVisibility = lerp(1.0f - length((float3)light.diffuse) / length(float3(1.0f, 1.0f, 1.0f)), 1.0f, visibility);

		diffuse += diffuseVisibility * (material.diffuse * light.diffuse * cosIncidence);
		specular += visibility * (material.specular * light.specular * phongTerm);
	}

	return ambient + diffuse + specular;
}