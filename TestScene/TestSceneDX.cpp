#include "TestSceneDX.h"

ID3DBlob* vertexShaderBuffer;
ID3DBlob* pixelShaderBuffer;
ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11Buffer* viewMarixBuffer;
float bg[4];

ID3D11Buffer* materialBuffer;

unsigned int modelMatrixBufferSlot = 0;
unsigned int viewMatrixBufferSlot = 1;
unsigned int projectionMatrixBufferSlot = 2;
unsigned int lightBufferSlot = 3;
unsigned int materialBufferSlot = 0;

std::vector<ModelDX> models;

const float rotDelta = 10.0f;
XMVECTOR up = { 0.0f, 1.0f, 0.0f };
XMVECTOR eye = { 5.0f, 5.0f, 8.0f };
XMVECTOR right = { 1.0f, 0.0f, 0.0f };
XMVECTOR center = { 0.0f, 0.0f, 0.0f };

TestSceneDX::TestSceneDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Scene";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
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
	viewMarixBuffer->Release();
}

bool TestSceneDX::InitScene()
{
	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	ID3D11RasterizerState1 *rasterizerState;
	D3D11_RASTERIZER_DESC1 rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;

	mDevice->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);
	mDeviceContext->RSSetState(rasterizerState);


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
	models.push_back(ModelDX("torus.bin", "torus.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixTranslation(-2.0f, 1.0f, -2.0f);
	models.push_back(ModelDX("sphere.bin", "sphere.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixTranslation(-2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX("sphere_smooth.bin", "sphere_smooth.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixTranslation(2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX("monkey.bin", "monkey.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	models.push_back(ModelDX("plane.bin", "plane.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	// PREPARE MATERIAL BUFFER
	D3D11_BUFFER_DESC materialDesc;
	ZeroMemory(&materialDesc, sizeof(materialDesc));
	materialDesc.ByteWidth = sizeof(Material);
	materialDesc.Usage = D3D11_USAGE_DEFAULT;
	materialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialDesc.CPUAccessFlags = 0;

	mDevice->CreateBuffer(&materialDesc, NULL, &materialBuffer);
	mDeviceContext->PSSetConstantBuffers(materialBufferSlot, 1, &materialBuffer);

	// UPLOAD LIGHTING
	//Light light;
	//light.position = Vector4(5.0f, 10.0f, 5.0f, 1.0f);
	//light.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	//light.diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	//light.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	//D3D11_BUFFER_DESC lightDesc;
	//ZeroMemory(&lightDesc, sizeof(lightDesc));
	//lightDesc.ByteWidth = sizeof(Light);
	//lightDesc.Usage = D3D11_USAGE_DEFAULT;
	//lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightDesc.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA lightData;
	//ZeroMemory(&lightData, sizeof(lightData));
	//lightData.pSysMem = &light;

	//ID3D11Buffer* lightBuffer;
	//mDevice->CreateBuffer(&lightDesc, &lightData, &lightBuffer);
	//mDeviceContext->VSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	//mDeviceContext->PSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	//lightBuffer->Release();

	Lighting lighting;
	lighting.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	Light point;
	point.position = Vector4(0.0f, 5.0f, 0.0f, 1.0f);
	point.diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	point.specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	point.halfDistance = 2.0f;

	Light directional;
	directional.position = Vector4(5.0f, 5.0f, 5.0f, 1.0f);
	directional.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	directional.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	lighting.lights[0] = point;
	lighting.lights[1] = directional;

	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	lightDesc.ByteWidth = sizeof(Lighting);
	lightDesc.Usage = D3D11_USAGE_DEFAULT;
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA lightData;
	ZeroMemory(&lightData, sizeof(lightData));
	lightData.pSysMem = &lighting;

	ID3D11Buffer* lightBuffer;
	mDevice->CreateBuffer(&lightDesc, &lightData, &lightBuffer);
	mDeviceContext->VSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	mDeviceContext->PSSetConstantBuffers(lightBufferSlot, 1, &lightBuffer);
	lightBuffer->Release();

	// PREPARE VIEW AND PROJECTION
	viewMarixBuffer = DXUtil::CreateEmptyMatrixBuffer(mDevice);
	mDeviceContext->VSSetConstantBuffers(viewMatrixBufferSlot, 1, &viewMarixBuffer);

	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(XMConvertToRadians(60.0f), 800 / 800, 1.0f, 500.0f);
	ID3D11Buffer* projectionMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, projectionMatrix);
	mDeviceContext->VSSetConstantBuffers(projectionMatrixBufferSlot, 1, &projectionMatrixBuffer);
	projectionMatrixBuffer->Release();

	return true;
}

void TestSceneDX::Update()
{
	float rotAmount = 0.0f;
	XMMATRIX rotMatrix = XMMatrixIdentity();
	if (input.right || input.left)
	{
		if (input.right)
			rotAmount = rotDelta;
		if (input.left)
			rotAmount = -rotDelta;
		rotMatrix = XMMatrixRotationAxis(up, XMConvertToRadians(rotAmount));
	}
	else if (input.up || input.down)
	{
		if (input.up)
			rotAmount = rotDelta;
		if (input.down)
			rotAmount = -rotDelta;
		rotMatrix = XMMatrixRotationAxis(right, XMConvertToRadians(rotAmount));
	}

	eye = XMVector3Transform(eye, rotMatrix);
	right = XMVector3Normalize(XMVector3Cross(up, (center - eye)));

	XMMATRIX viewMarix = XMMatrixLookAtRH(eye, center, up);
	mDeviceContext->UpdateSubresource(viewMarixBuffer, 0, NULL, &viewMarix, 0, 0);
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