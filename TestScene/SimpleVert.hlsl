struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 lightDirectionViewSpace : TEXCOORD1;
	float3 viewDirectionViewSpace : TEXCOORD2;
};

struct Light
{
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

cbuffer ModelMatrix : register(b0)
{
	float4x4 modelMatrix;
};

cbuffer ViewMatrix : register(b1)
{
	float4x4 viewMatrix;
};

cbuffer ProjectionMatrix : register(b2)
{
	float4x4 projectionMatrix;
};

cbuffer Light : register(b3)
{
	Light light;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;

	float4 positionViewSpace = mul(viewMatrix, mul(modelMatrix, inPos));
	output.position = mul(projectionMatrix, positionViewSpace);

	output.normalViewSpace = mul(viewMatrix, mul(modelMatrix, inNormal));

	float3 lightPosViewSpace = mul(viewMatrix, light.position);
	output.lightDirectionViewSpace = lightPosViewSpace - positionViewSpace;

	output.viewDirectionViewSpace = -positionViewSpace;

	return output;
}