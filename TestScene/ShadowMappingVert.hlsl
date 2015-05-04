struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

cbuffer ModelMatrix : register(b0)
{
	float4x4 modelMatrix;
};

cbuffer ViewProjectionMatrix : register(b4)
{
	float4x4 viewProjectionMatrix;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION)
{
	VS_OUTPUT output;
	output.position = mul(viewProjectionMatrix, mul(modelMatrix, inPos));
	return output;
}