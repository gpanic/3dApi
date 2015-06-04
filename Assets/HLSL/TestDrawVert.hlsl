struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 origPos : TEXCOORD0;
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
	output.origPos = inPos;

	return output;
}