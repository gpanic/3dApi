#include "SimpleUniformInclude.fx"

cbuffer OverrideColor : register(b0)
{
	float4 overrideColor;
};

cbuffer ColorsBlock : register(b1)
{
	float4 colorA;
	float4 colorB;
};

float4 pixelShader(VS_OUTPUT input) : SV_TARGET
{
	return overrideColor + colorA + colorB;
}