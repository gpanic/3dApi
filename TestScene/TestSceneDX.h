#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "ObjReader.h"
#include "ModelDX.h"
#include "Lighting.h"
using namespace DirectX;

class TestSceneDX : public DXApp
{
public:
	TestSceneDX(HINSTANCE hInstance);;
	virtual ~TestSceneDX();
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
	ID3D11Buffer* viewMarixBuffer;
	float bg[4];

	ID3D11Buffer* materialBuffer;

	unsigned int modelMatrixBufferSlot = 0;
	unsigned int viewMatrixBufferSlot = 1;
	unsigned int projectionMatrixBufferSlot = 2;
	unsigned int lightBufferSlot = 3;
	unsigned int materialBufferSlot = 0;

	std::vector<ModelDX> models;

	const float rotDelta = 10.0f;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR eye = XMVectorSet(5.0f, 5.0f, 8.0f, 1.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

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

