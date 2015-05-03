static const int numberOfLights = 2;

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normalViewSpace : TEXCOORD0;
	float3 positionViewSpace : TEXCOORD1;
	float3 lightPositionsViewSpace[numberOfLights] : TEXCOORD2;
};

struct Light
{
	float4 position;
	float4 diffuse;
	float4 specular;
	float halfDistance;
};

struct Lighting
{
	float4 ambient;
	Light lights[numberOfLights];
};

cbuffer ModelMatrix : register(b0)
{
	float4x4 modelMatrix;
};

cbuffer ViewMatrix : register(b1)
{
	float4x4 viewMatrix;
};

cbuffer ProjectionMatrix : register(b2)
{
	float4x4 projectionMatrix;
};

cbuffer Lighting : register(b3)
{
	Lighting lighting;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION, float3 inNormal : NORMAL)
{
	VS_OUTPUT output;

	float4 positionViewSpace = mul(viewMatrix, mul(modelMatrix, inPos));
	output.positionViewSpace = positionViewSpace;
	output.normalViewSpace = mul(viewMatrix, mul(modelMatrix, inNormal));

	for (int i = 0; i < numberOfLights; ++i)
	{
		output.lightPositionsViewSpace[i] = mul(viewMatrix, lighting.lights[i].position);
	}

	output.position = mul(projectionMatrix, positionViewSpace);

	return output;
}