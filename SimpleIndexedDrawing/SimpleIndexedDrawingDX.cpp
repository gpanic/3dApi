#include "SimpleIndexedDrawingDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11Buffer* indexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

SimpleIndexedDrawingDX::SimpleIndexedDrawingDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple Indexed Drawing";
}

SimpleIndexedDrawingDX::~SimpleIndexedDrawingDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	inputLayout->Release();
}

bool SimpleIndexedDrawingDX::InitScene()
{
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	D3DReadFileToBlob(L"SimpleIndexedDrawingVert.cso", &vertexShaderBuffer);
	D3DReadFileToBlob(L"SimpleIndexedDrawingFrag.cso", &pixelShaderBuffer);

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
	mDevice->CreateInputLayout(vertexLayout, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(Data::indices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = Data::indices;

	mDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	return true;
}

void SimpleIndexedDrawingDX::Update()
{
}

void SimpleIndexedDrawingDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT stride = 8 * sizeof(float);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->DrawIndexed(Data::indexCount, 0, 0);
}