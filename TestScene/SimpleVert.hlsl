struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
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

VS_OUTPUT vertexShader(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;

	float4 position = inPos;
	position = mul(modelMatrix, position);
	position = mul(viewMatrix, position);
	position = mul(projectionMatrix, position);
	output.position = position;
	output.normal = inNormal;

	return output;
}