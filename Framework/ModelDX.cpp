#include "ModelDX.h"

ModelDX::ModelDX() : vertexBuffer(nullptr), inputLayout(nullptr), vertexCount(0)
{
}

ModelDX::ModelDX(const std::string &objPath, const std::string &mtlPath, ID3D11Device1 *device, ID3DBlob* vertexShaderBuffer, const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout, const XMMATRIX &matrix, bool binary)
{
	std::vector<Vertex> vertices;
	if (!binary)
	{
		ObjReader::Read(objPath, mtlPath, vertices, material);
	}
	else
	{
		std::string materialName;
		BinaryIO::ReadVertices(objPath, vertices, materialName);
		ObjReader::ReadMtl(mtlPath, materialName, material);
	}
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
	device->CreateInputLayout(&layout[0], layout.size() , vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	modelMatrixBuffer = DXUtil::CreateMatrixBuffer(device, matrix);
}

void ModelDX::Release()
{
	vertexBuffer->Release();
	inputLayout->Release();
	modelMatrixBuffer->Release();
}