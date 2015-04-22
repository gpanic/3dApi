#include "SimpleTexturingInclude.fx"

Texture2D tex;
SamplerState ss;

float4 pixelShader(VertexOutput input) : SV_TARGET
{
	return tex.Sample(ss, input.uv);
}