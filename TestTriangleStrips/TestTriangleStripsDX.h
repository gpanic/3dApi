#pragma once
#include "DXApp.h"
#include "ObjReader.h"
#include "BinaryIO.h"
#include "Material.h"
#include "Lighting.h"

class TestTriangleStripsDX : public DXApp
{
public:
	TestTriangleStripsDX(HINSTANCE hInstance);;
	virtual ~TestTriangleStripsDX();

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

	ID3D11Buffer* viewMatrixBuffer;
	std::vector<Vector4> vertices;

	const float rotDelta = 5.0f;
	XMFLOAT4 up;
	XMFLOAT4 eye;
	XMFLOAT4 right;
	XMFLOAT4 center;
};

