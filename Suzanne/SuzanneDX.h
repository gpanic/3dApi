#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "ObjReader.h"
#include "ModelDX.h"
#include "Lighting.h"
using namespace DirectX;

class SuzanneDX : public DXApp
{
public:
	SuzanneDX(HINSTANCE hInstance);;
	virtual ~SuzanneDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	void RenderShadowMaps();

	static const int NUMBER_OF_LIGHTS = 2;

	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* viewMatrixBuffer;
	float bg[4];

	ID3D11Buffer* materialBuffer;

	unsigned int modelMatrixBufferSlot = 0;
	unsigned int viewMatrixBufferSlot = 1;
	unsigned int projectionMatrixBufferSlot = 2;
	unsigned int lightBufferSlot = 3;
	unsigned int materialBufferSlot = 0;

	std::vector<ModelDX> models;

	const float rotDelta = 10.0f;
	XMFLOAT4 up;
	XMFLOAT4 eye;
	XMFLOAT4 right;
	XMFLOAT4 center;

	// LIGHTING
	Lighting lighting;

	// SHADOW MAPPING
	const int SHADOW_RESOLUTION = 1024;
	ID3DBlob* shadowVertexShaderBuffer;
	ID3DBlob* shadowPixelShaderBuffer;
	ID3D11VertexShader* shadowVertexShader;
	ID3D11PixelShader* shadowPixelShader;
	unsigned int shadowViewProjectionMatrixBufferSlot = 4;
	unsigned int shadowBiasMatrixBufferSlot = 5;
	ID3D11ShaderResourceView* shadowMapResources[NUMBER_OF_LIGHTS];
	ID3D11SamplerState* shadowMapSamplerState;
};

