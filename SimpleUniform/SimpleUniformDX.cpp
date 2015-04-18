#include "SimpleUniformDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

SimpleUniformDX::SimpleUniformDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple";
}

SimpleUniformDX::~SimpleUniformDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	inputLayout->Release();
}

bool SimpleUniformDX::InitScene()
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

	D3DReadFileToBlob(L"SimpleUniformVert.cso", &vertexShaderBuffer);
	D3DReadFileToBlob(L"SimpleUniformFrag.cso", &pixelShaderBuffer);

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

	float overrideColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	D3D11_BUFFER_DESC overrideColorDesc;
	ZeroMemory(&overrideColorDesc, sizeof(overrideColorDesc));
	overrideColorDesc.ByteWidth = sizeof(overrideColor);
	overrideColorDesc.Usage = D3D11_USAGE_DYNAMIC;
	overrideColorDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	overrideColorDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA overrideColorData;
	ZeroMemory(&overrideColorData, sizeof(overrideColorData));
	overrideColorData.pSysMem = &overrideColor;

	ID3D11Buffer* overrideColorBuffer;
	mDevice->CreateBuffer(&overrideColorDesc, &overrideColorData, &overrideColorBuffer);
	mDeviceContext->PSSetConstantBuffers(0, 1, &overrideColorBuffer);
	
	float colorsBlock[8] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0f };

	D3D11_BUFFER_DESC colorsBlockDesc;
	ZeroMemory(&colorsBlockDesc, sizeof(colorsBlockDesc));
	colorsBlockDesc.ByteWidth = sizeof(colorsBlock);
	colorsBlockDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorsBlockDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorsBlockDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA colorsBlockData;
	ZeroMemory(&colorsBlockData, sizeof(colorsBlockData));
	colorsBlockData.pSysMem = &colorsBlock;

	ID3D11Buffer* colorsBlockBuffer;
	mDevice->CreateBuffer(&colorsBlockDesc, &colorsBlockData, &colorsBlockBuffer);
	mDeviceContext->PSSetConstantBuffers(1, 1, &colorsBlockBuffer);

	return true;
}

void SimpleUniformDX::Update()
{
}

void SimpleUniformDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);

	UINT stride = 8 * sizeof(float);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->Draw(3, 0);
}