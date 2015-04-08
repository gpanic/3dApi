#include "SimpleInclude.fx"

VS_OUTPUT vertexShader(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUTPUT output;

	output.Pos = inPos;
	output.Color = inColor;

	return output;
}