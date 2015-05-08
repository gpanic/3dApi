struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
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

VS_OUTPUT vertexShader(float4 inPos : POSITION)
{
	VS_OUTPUT output;

	output.position = mul(projectionMatrix, mul(viewMatrix, mul(modelMatrix, inPos)));

	float3 color1 = lerp(float3(1, 0, 0), float3(0, 1, 0), ((inPos.x + 10.0f) / 20.0f));
	float3 color2 = lerp(float3(0, 0, 1), float3(1, 1, 1), ((inPos.x + 10.0f) / 20.0f));
	output.color = lerp(color1, color2, ((inPos.z + 10.0f) / 20.0f));

	return output;
}