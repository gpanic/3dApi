struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUTPUT output;

	output.Pos = inPos;
	output.Color = inColor;

	return output;
}