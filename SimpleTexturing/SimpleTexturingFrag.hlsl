#include "SimpleTexturingInclude.fx"

Texture2D tex : register(t0);
SamplerState ss : register(s0);

float4 pixelShader(VertexOutput input) : SV_TARGET
{
	return tex.Sample(ss, input.uv);
}