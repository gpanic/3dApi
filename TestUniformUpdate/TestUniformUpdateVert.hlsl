struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUTPUT output;

	output.Pos = inPos;

	return output;
}