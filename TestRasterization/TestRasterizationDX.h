#pragma once
#include "DXApp.h"
#include "ObjReader.h"
#include "BinaryIO.h"
#include "Material.h"
#include "Lighting.h"

class TestRasterizationDX : public DXApp
{
public:
	TestRasterizationDX(HINSTANCE hInstance);;
	virtual ~TestRasterizationDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11InputLayout* inputLayout;
	float bg[4];

	unsigned int modelMatrixBufferSlot = 0;
	unsigned int viewMatrixBufferSlot = 1;
	unsigned int projectionMatrixBufferSlot = 2;
	unsigned int lightBufferSlot = 3;
	unsigned int ambientBufferSlot = 4;
	unsigned int materialBufferSlot = 5;

	ID3D11Buffer* materialBuffer;
	ID3D11Buffer* viewMatrixBuffer;
	Material material;
	std::vector<Vertex> vertices;
	std::vector<Vector3> offsets;

	const float rotDelta = 5.0f;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR eye = XMVectorSet(0.0f, 20.0f, 60.0f, 1.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR center = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
};

