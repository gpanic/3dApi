#include "ModelDX.h"

ModelDX::ModelDX() : vertexBuffer(nullptr), inputLayout(nullptr), vertexCount(0)
{
}

ModelDX::ModelDX(const std::string &objPath, const std::string &mtlPath, ID3D11Device1 *device, ID3DBlob* vertexShaderBuffer, const D3D11_INPUT_ELEMENT_DESC *layout, int elements)
{
	std::vector<Vertex> vertices;
	Material mat;
	ObjReader::Read(objPath, mtlPath, vertices, mat);
	vertexCount = vertices.size();

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = vertices.size() * sizeof(Vertex);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	device->CreateInputLayout(layout, elements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);
}

void ModelDX::Release()
{
	vertexBuffer->Release();
	inputLayout->Release();
}