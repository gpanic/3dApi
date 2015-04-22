#include "SimpleTexturingInclude.fx"

VertexOutput vertexShader(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	VertexOutput output;

	output.pos = pos;
	output.uv = uv;

	return output;
}