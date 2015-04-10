#include "SimpleDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

SimpleDX::SimpleDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple";
}

SimpleDX::~SimpleDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	inputLayout->Release();
}

bool SimpleDX::InitScene()
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

	D3DReadFileToBlob(L"SimpleVert.cso", &vertexShaderBuffer);
	D3DReadFileToBlob(L"SimpleFrag.cso", &pixelShaderBuffer);

	mDevice1->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice1->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = 3 * 8 * sizeof(float);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = Data::vertices;

	mDevice1->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	mDevice1->CreateInputLayout(vertexLayout, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	return true;
}

void SimpleDX::Update()
{
}

void SimpleDX::Render()
{
	mDeviceContext1->ClearRenderTargetView(mRenderTargetView, bg);

	UINT stride = 8 * sizeof(float);
	UINT offset = 0;
	mDeviceContext1->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext1->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext1->IASetInputLayout(inputLayout);

	mDeviceContext1->VSSetShader(vertexShader, 0, 0);
	mDeviceContext1->PSSetShader(pixelShader, 0, 0);

	mDeviceContext1->Draw(3, 0);
}