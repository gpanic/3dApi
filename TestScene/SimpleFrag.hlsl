struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 lightDirectionViewSpace : TEXCOORD1;
	float3 viewDirectionViewSpace : TEXCOORD2;
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
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

cbuffer Material : register(b0)
{
	Material material;
};

cbuffer Light : register(b3)
{
	Light light;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	float3 normalViewSpace = normalize(input.normalViewSpace);
	float3 lightDirectionViewSpace = normalize(input.lightDirectionViewSpace);
	float3 viewDirectionViewSpace = normalize(input.viewDirectionViewSpace);

	float3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);

	float cosIncidence = dot(normalViewSpace, normalize(lightDirectionViewSpace));
	cosIncidence = saturate(cosIncidence);

	float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
	phongTerm = saturate(phongTerm);
	phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, material.shininess);

	return (material.ambient * light.ambient) + (material.diffuse * light.diffuse * cosIncidence) + (material.specular * light.specular * phongTerm);
}