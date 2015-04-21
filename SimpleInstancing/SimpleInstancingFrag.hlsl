#include "SimpleInstancingInclude.fx"

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return input.Color;
}