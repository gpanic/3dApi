#pragma once
#include "DXApp.h"
#include <DirectXMath.h>
#include "BinaryIO.h"

class TestDrawDX : public DXApp
{
public:
	TestDrawDX(HINSTANCE hInstance);;
	virtual ~TestDrawDX();
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
	XMFLOAT4 up;
	XMFLOAT4 eye;
	XMFLOAT4 right;
	XMFLOAT4 center;
};

