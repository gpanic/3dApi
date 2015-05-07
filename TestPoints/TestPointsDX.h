#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "BinaryIO.h"

class TestPointsDX : public DXApp
{
public:
	TestPointsDX(HINSTANCE hInstance);;
	virtual ~TestPointsDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;
	ID3D11InputLayout* inputLayout;
	float bg[4];

	unsigned int modelMatrixBufferSlot = 0;
	unsigned int viewMatrixBufferSlot = 1;
	unsigned int projectionMatrixBufferSlot = 2;

	std::vector<Vector4> verts;

	ID3D11Buffer* viewMatrixBuffer;

	const float rotDelta = 5.0f;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR eye = XMVectorSet(2.2f, 2.2f, 2.2f, 1.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
};

