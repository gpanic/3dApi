static const int numberOfLights = 2;

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 positionViewSpace : TEXCOORD1;
	float3 lightPositionsViewSpace[numberOfLights] : TEXCOORD2;
	float4 shadowCoord : TEXCOORD4;
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

Texture2D shadowMap : register(t0);
SamplerState shadowMapSampler : register(s0);

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	float3 normalViewSpace = normalize(input.normalViewSpace);

	float4 ambient = material.ambient * lighting.ambient;
	float4 diffuse = 0.0f;
	float4 specular = 0.0f;
	for (int i = 1; i < numberOfLights; ++i)
	{
		Light light = lighting.lights[i];

		float3 lightDirectionViewSpace = light.position.w == 0.0f ? input.lightPositionsViewSpace[i] : input.lightPositionsViewSpace[i] - input.positionViewSpace;
		lightDirectionViewSpace = normalize(lightDirectionViewSpace);
		float3 viewDirectionViewSpace = normalize(-input.positionViewSpace);

		float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
		cosIncidence = saturate(cosIncidence);

		float3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);
		float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
		phongTerm = saturate(phongTerm);
		phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
		phongTerm = pow(phongTerm, material.shininess);

		float3 difference = input.lightPositionsViewSpace[i] - input.positionViewSpace;

		float attenuation = 1.0f;
		if (light.position.w != 0.0f)
			attenuation = 1.0f / (1.0f + (1.0f / (light.halfDistance * light.halfDistance)) * length(difference));

		diffuse += attenuation * (material.diffuse * light.diffuse * cosIncidence);
		specular += attenuation * (material.specular * light.specular * phongTerm);
	}

	float visibility = 1.0f;
	float2 uv = input.shadowCoord.xy;
	uv.y = 1 - uv.y;
	if (shadowMap.Sample(shadowMapSampler, uv).x < input.shadowCoord.z)
	{
		visibility = 0.0f;
	}

	return ambient + visibility * diffuse + visibility * specular;
	//return ambient + diffuse + specular;
}