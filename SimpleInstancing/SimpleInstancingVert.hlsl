#include "SimpleInstancingInclude.fx"

VS_OUTPUT vertexShader(float4 inPos : POSITION, float4 inOffset : OFFSET, float4 inColor : COLOR)
{
	VS_OUTPUT output;

	output.Pos = inPos + inOffset;
	output.Color = inColor;

	return output;
}