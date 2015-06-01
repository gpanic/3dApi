#include "TestUniformUpdateDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

ID3D11Buffer* colorsBlockBuffer;

TestUniformUpdateDX::TestUniformUpdateDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Uniform Update";
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

TestUniformUpdateDX::~TestUniformUpdateDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	inputLayout->Release();
}

bool TestUniformUpdateDX::InitScene()
{
	const float vertices[] =
	{
		-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	D3DCompileFromFile(L"TestUniformUpdateVert.hlsl", NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(L"TestUniformUpdateFrag.hlsl", NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices;

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	mDevice->CreateInputLayout(vertexLayout, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	const float color[]
	{
		0.0f, 0.0f, 0.0f, 1.0f
	};

	D3D11_BUFFER_DESC colorsBlockDesc;
	ZeroMemory(&colorsBlockDesc, sizeof(colorsBlockDesc));
	colorsBlockDesc.ByteWidth = sizeof(color);
	colorsBlockDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorsBlockDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorsBlockDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA colorsBlockData;
	ZeroMemory(&colorsBlockData, sizeof(colorsBlockData));
	colorsBlockData.pSysMem = &color;

	mDevice->CreateBuffer(&colorsBlockDesc, &colorsBlockData, &colorsBlockBuffer);
	mDeviceContext->PSSetConstantBuffers(0, 1, &colorsBlockBuffer);

	return true;
}

void TestUniformUpdateDX::Update()
{
	float color2[]
	{
		1.0f, 1.0f, 0.0f, 1.0f
	};

	//for (int i = 0; i < 50000; ++i)
	//{
	//	mDeviceContext->UpdateSubresource(colorsBlockBuffer, 0, NULL, color2, 0, 0);
	//}

	for (int i = 0; i < 50000; ++i)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(res));
		mDeviceContext->Map(colorsBlockBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		float *ptr = (float *)res.pData;
		memcpy(ptr, color2, sizeof(color2));
		mDeviceContext->Unmap(colorsBlockBuffer, 0);
	}
}

void TestUniformUpdateDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT stride = 8 * sizeof(float);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->Draw(3, 0);
}