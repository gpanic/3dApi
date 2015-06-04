struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

float4 pixelShader(VS_OUTPUT input) : SV_Target
{
	return float4(input.position.z, input.position.z, input.position.z, 1);
}