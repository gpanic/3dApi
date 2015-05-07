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

VS_OUTPUT vertexShader(float4 inPos : POSITION, float3 inNormal : NORMAL, float3 inOffset : OFFSET)
{
	VS_OUTPUT output;

	float4 posViewSpace = mul(viewMatrix, (mul(modelMatrix, inPos) + float4(inOffset, 0.0f)));
	output.positionViewSpace = (float3)posViewSpace;
	output.normalViewSpace = mul((float3x3)viewMatrix, mul((float3x3)modelMatrix, inNormal));
	output.lightDirViewSpace = mul((float3x3)viewMatrix, light.position);
	output.position = mul(projectionMatrix, mul(viewMatrix, (mul(modelMatrix, inPos) + float4(inOffset, 0.0f))));

	return output;
}