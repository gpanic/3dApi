#include "TestSceneDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;
float bg[4];

std::vector<Vertex> verts;
Material mat;

ModelDX model;

XMVECTOR up = { 0.0f, 1.0f, 0.0f };
XMVECTOR eye = { 0.0f, 0.0f, -8.0f };
XMVECTOR center = { 0.0f, 0.0f, 0.0f };

TestSceneDX::TestSceneDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Simple";
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

TestSceneDX::~TestSceneDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	model.Release();
	//vertexBuffer->Release();
	//inputLayout->Release();
}

bool TestSceneDX::InitScene()
{
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int vertexLayoutElements = 2;

	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	D3DCompileFromFile(L"SimpleVert.hlsl", NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(L"SimpleFrag.hlsl", NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	//D3DReadFileToBlob(L"SimpleVert.cso", &vertexShaderBuffer);
	//D3DReadFileToBlob(L"SimpleFrag.cso", &pixelShaderBuffer);

	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	//float vertices[] =
	//{
	//	-0.75f, -0.75f, 0.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f,

	//	0.0f, 0.75f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f,

	//	0.75f, -0.75f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 1.0f,
	//};

	model = ModelDX("sphere_smooth.obj", "sphere_smooth.mtl", mDevice, vertexShaderBuffer, vertexLayout, vertexLayoutElements);

	// MODEL MATRIX
	XMMATRIX modelMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	D3D11_BUFFER_DESC modelMatrixDesc;
	ZeroMemory(&modelMatrixDesc, sizeof(modelMatrixDesc));
	modelMatrixDesc.ByteWidth = sizeof(modelMatrix);
	modelMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	modelMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	modelMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA modelMatrixData;
	ZeroMemory(&modelMatrixData, sizeof(modelMatrixData));
	modelMatrixData.pSysMem = &modelMatrix;

	ID3D11Buffer* modelMatrixBuffer;
	mDevice->CreateBuffer(&modelMatrixDesc, &modelMatrixData, &modelMatrixBuffer);
	mDeviceContext->VSSetConstantBuffers(0, 1, &modelMatrixBuffer);
	mDeviceContext->PSSetConstantBuffers(0, 1, &modelMatrixBuffer);

	// VIEW MATRIX
	XMMATRIX viewMarix = XMMatrixLookAtLH(eye, center, up);

	D3D11_BUFFER_DESC viewMarixDesc;
	ZeroMemory(&viewMarixDesc, sizeof(viewMarixDesc));
	viewMarixDesc.ByteWidth = sizeof(viewMarix);
	viewMarixDesc.Usage = D3D11_USAGE_DYNAMIC;
	viewMarixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	viewMarixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA viewMarixData;
	ZeroMemory(&viewMarixData, sizeof(viewMarixData));
	viewMarixData.pSysMem = &viewMarix;

	ID3D11Buffer* viewMarixBuffer;
	mDevice->CreateBuffer(&viewMarixDesc, &viewMarixData, &viewMarixBuffer);
	mDeviceContext->VSSetConstantBuffers(1, 1, &viewMarixBuffer);
	mDeviceContext->PSSetConstantBuffers(1, 1, &viewMarixBuffer);

	// PROJECTION MATRIX
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 800 / 800, 1.0f, 500.0f);

	D3D11_BUFFER_DESC projectionMatrixDesc;
	ZeroMemory(&projectionMatrixDesc, sizeof(projectionMatrixDesc));
	projectionMatrixDesc.ByteWidth = sizeof(projectionMatrix);
	projectionMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	projectionMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	projectionMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA projectionMatrixData;
	ZeroMemory(&projectionMatrixData, sizeof(projectionMatrixData));
	projectionMatrixData.pSysMem = &projectionMatrix;

	ID3D11Buffer* projectionMatrixBuffer;
	mDevice->CreateBuffer(&projectionMatrixDesc, &projectionMatrixData, &projectionMatrixBuffer);
	mDeviceContext->VSSetConstantBuffers(2, 1, &projectionMatrixBuffer);
	mDeviceContext->PSSetConstantBuffers(2, 1, &projectionMatrixBuffer);

	return true;
}

void TestSceneDX::Update()
{
}

void TestSceneDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(model.inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->Draw(model.vertexCount, 0);
}