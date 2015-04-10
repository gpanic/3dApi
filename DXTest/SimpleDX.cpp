#include "SimpleDX.h"

ID3DBlob* VS_Buffer;
ID3DBlob* PS_Buffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;

SimpleDX::SimpleDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple";
}

SimpleDX::~SimpleDX()
{
}

bool SimpleDX::InitScene()
{
	float vertexData[] = {
		-0.75f, -0.75f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.75f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.75f, -0.75f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DReadFileToBlob(L"SimpleVert.cso", &VS_Buffer);
	D3DReadFileToBlob(L"SimpleFrag.cso", &PS_Buffer);

	mDevice->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	mDevice->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	mDeviceContext->VSSetShader(VS, 0, 0);
	mDeviceContext->PSSetShader(PS, 0, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = 3 * 8 * sizeof(float);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertexData;

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	UINT stride = 8 * sizeof(float);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	mDevice->CreateInputLayout(vertexLayout, 2, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &inputLayout);

	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float bg[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	return true;
}

void SimpleDX::Update()
{
}

void SimpleDX::Render()
{
	
	mDeviceContext->Draw(3, 0);
}