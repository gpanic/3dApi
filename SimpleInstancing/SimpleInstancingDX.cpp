#include "SimpleInstancingDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11Buffer* instanceBuffer;
ID3D11Buffer* indexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

SimpleInstancingDX::SimpleInstancingDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple Instancing";
}

SimpleInstancingDX::~SimpleInstancingDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	instanceBuffer->Release();
	indexBuffer->Release();
	inputLayout->Release();
}

bool SimpleInstancingDX::InitScene()
{
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "OFFSET", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 4 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	D3DReadFileToBlob(L"SimpleInstancingVert.cso", &vertexShaderBuffer);
	D3DReadFileToBlob(L"SimpleInstancingFrag.cso", &pixelShaderBuffer);

	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(Data::vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = Data::vertices;

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.ByteWidth = sizeof(Data::instanceData);
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA instanceBufferData;
	ZeroMemory(&instanceBufferData, sizeof(instanceBufferData));
	instanceBufferData.pSysMem = Data::instanceData;

	mDevice->CreateBuffer(&instanceBufferDesc, &instanceBufferData, &instanceBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(Data::indices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = Data::indices;

	mDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	mDevice->CreateInputLayout(vertexLayout, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	return true;
}

void SimpleInstancingDX::Update()
{
}

void SimpleInstancingDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11Buffer* buffers[2] = { vertexBuffer, instanceBuffer };
	UINT strides[2] = { 4 * sizeof(float), 2 * 4 * sizeof(float) };
	UINT offsets[2] = { 0, 0 };
	mDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	mDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->DrawIndexedInstanced(Data::indexCount, Data::instanceCount, 0, 0, 0);
}