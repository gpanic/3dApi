struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return float4(input.color, 1);
}