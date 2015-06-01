struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

cbuffer ColorBlock : register(b0)
{
	float4 color;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return color;
}