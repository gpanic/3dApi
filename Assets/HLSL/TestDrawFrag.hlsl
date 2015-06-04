struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 origPos : TEXCOORD0;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return float4((input.origPos.x + 1.0f) / 2.0f,
	(input.origPos.y + 1.0f) / 2.0f,
	(input.origPos.z + 1.0f) / 2.0f,
	1.0f);
}