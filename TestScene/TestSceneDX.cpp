#include "TestSceneDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
float bg[4];

ID3D11Buffer* materialBuffer;

std::vector<ModelDX> models;

XMVECTOR up = { 0.0f, 1.0f, 0.0f };
XMVECTOR eye = { 5.0f, 5.0f, -8.0f };
XMVECTOR center = { 0.0f, 0.0f, 0.0f };

unsigned int modelMatrixBufferSlot = 0;
unsigned int viewMatrixBufferSlot = 1;
unsigned int projectionMatrixBufferSlot = 2;
unsigned int lightBufferSlot = 3;
unsigned int materialBufferSlot = 0;

TestSceneDX::TestSceneDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Scene";
	mBenchmarkResultName = mAppTitle + " Result.txt";
}

TestSceneDX::~TestSceneDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	materialBuffer->Release();
	for (ModelDX model : models)
	{
		model.Release();
	}
}

bool TestSceneDX::InitScene()
{
	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	// COMPILE SHADERS
	D3DCompileFromFile(L"SimpleVert.hlsl", NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(L"SimpleFrag.hlsl", NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	// PREPARE MODELS
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	vertexLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex().position), D3D11_INPUT_PER_VERTEX_DATA, 0 });

	XMMATRIX modelMatrix;

	modelMatrix = XMMatrixTranslation(2.0f, 1.0f, -2.0f);
	models.push_back(ModelDX("torus.obj", "torus.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix));

	modelMatrix = XMMatrixTranslation(-2.0f, 1.0f, -2.0f);
	models.push_back(ModelDX("sphere.obj", "sphere.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix));

	modelMatrix = XMMatrixTranslation(-2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX("sphere_smooth.obj", "sphere_smooth.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix));

	/*modelMatrix = XMMatrixTranslation(2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX("monkey.obj", "monkey.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix));*/

	modelMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixScaling(5.0f, 5.0f, 5.0f);
	models.push_back(ModelDX("plane.obj", "plane.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix));

	// PREPARE MATERIAL BUFFER
	D3D11_BUFFER_DESC materialDesc;
	ZeroMemory(&materialDesc, sizeof(materialDesc));
	materialDesc.ByteWidth = sizeof(Material);
	materialDesc.Usage = D3D11_USAGE_DEFAULT;
	materialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialDesc.CPUAccessFlags = 0;

	mDevice->CreateBuffer(&materialDesc, NULL, &materialBuffer);
	mDeviceContext->PSSetConstantBuffers(materialBufferSlot, 1, &materialBuffer);

	// UPLOAD LIGHT
	Light light;
	light.position = Vector4(5.0f, 10.0f, 5.0f, 1.0f);
	light.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	light.diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	light.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	lightDesc.ByteWidth = sizeof(Light);
	lightDesc.Usage = D3D11_USAGE_DEFAULT;
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA lightData;
	ZeroMemory(&lightData, sizeof(lightData));
	lightData.pSysMem = &light;

	ID3D11Buffer* lightBuffer;
	mDevice->CreateBuffer(&lightDesc, &lightData, &lightBuffer);
	mDeviceContext->VSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	mDeviceContext->PSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	lightBuffer->Release();

	// VIEW MATRIX
	XMMATRIX viewMarix = XMMatrixLookAtLH(eye, center, up);
	ID3D11Buffer* viewMarixBuffer = DXUtil::CreateMatrixBuffer(mDevice, viewMarix);
	mDeviceContext->VSSetConstantBuffers(viewMatrixBufferSlot, 1, &viewMarixBuffer);
	viewMarixBuffer->Release();

	// PROJECTION MATRIX
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 800 / 800, 1.0f, 500.0f);
	ID3D11Buffer* projectionMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, projectionMatrix);
	mDeviceContext->VSSetConstantBuffers(projectionMatrixBufferSlot, 1, &projectionMatrixBuffer);
	projectionMatrixBuffer->Release();

	return true;
}

void TestSceneDX::Update()
{
}

void TestSceneDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (ModelDX model : models)
	{
		mDeviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mDeviceContext->IASetInputLayout(model.inputLayout);

		mDeviceContext->VSSetShader(vertexShader, 0, 0);
		mDeviceContext->PSSetShader(pixelShader, 0, 0);

		mDeviceContext->VSSetConstantBuffers(modelMatrixBufferSlot, 1, &model.modelMatrixBuffer);
		mDeviceContext->UpdateSubresource(materialBuffer, 0, NULL, &model.material, 0, 0);

		mDeviceContext->Draw(model.vertexCount, 0);
	}

}