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

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}