struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 positionViewSpace : TEXCOORD1;
	float3 lightDirViewSpace : TEXCOORD2;
};

struct Light
{
	float4 position;
	float4 diffuse;
	float4 specular;
	float halfDistance;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shininess;
	float3 padding;
};

cbuffer Light : register(b3)
{
	Light light;
};

cbuffer Ambient : register(b4)
{
	float4 ambient;
};

cbuffer Material : register(b5)
{
	Material material;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	float3 normalViewSpace = normalize(input.normalViewSpace);
	float3 viewDirectionViewSpace = normalize(-input.positionViewSpace);
	float3 lightDirectionViewSpace = normalize(input.lightDirViewSpace);
	float3 reflectDirectionViewSpace = reflect(-lightDirectionViewSpace, normalViewSpace);

	float cosIncidence = dot(normalViewSpace, lightDirectionViewSpace);
	cosIncidence = saturate(cosIncidence);

	float phongTerm = dot(viewDirectionViewSpace, reflectDirectionViewSpace);
	phongTerm = saturate(phongTerm);
	phongTerm = cosIncidence != 0.0 ? phongTerm : 0.0;
	phongTerm = pow(phongTerm, material.shininess);

	return ambient + (material.diffuse * light.diffuse * cosIncidence) + (material.specular * light.specular * phongTerm);
}