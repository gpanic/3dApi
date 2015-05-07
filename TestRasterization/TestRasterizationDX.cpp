#include "TestRasterizationDX.h"

TestRasterizationDX::TestRasterizationDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Rasterization";
	mBenchmarkResultName = mAppTitle + " Result.txt";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

TestRasterizationDX::~TestRasterizationDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	instanceBuffer->Release();
	inputLayout->Release();
}

bool TestRasterizationDX::InitScene()
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
	rasterizerState->Release();

	ObjReader::Read("sphere_smooth.obj", "sphere_smooth.mtl", vertices, material);
	BinaryIO::ReadVector3s("verts.bin", offsets);

	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex().position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "OFFSET", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	D3DCompileFromFile(L"TestRasterizationVert.hlsl", NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(L"TestRasterizationFrag.hlsl", NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = vertices.size() * sizeof(Vertex);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(instanceBufferDesc));
	instanceBufferDesc.ByteWidth = offsets.size() * sizeof(Vector3);
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA instanceBufferData;
	ZeroMemory(&instanceBufferData, sizeof(instanceBufferData));
	instanceBufferData.pSysMem = &offsets[0];

	mDevice->CreateBuffer(&instanceBufferDesc, &instanceBufferData, &instanceBuffer);

	mDevice->CreateInputLayout(vertexLayout, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	// UPLOAD LIGHT
	Light light;
	light.position = Vector4(5.0f, 5.0f, 5.0f, 0.0f);
	light.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 ambient(0.1f, 0.1f, 0.1f, 1.0f);

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

	D3D11_BUFFER_DESC ambientDesc;
	ZeroMemory(&ambientDesc, sizeof(ambientDesc));
	ambientDesc.ByteWidth = sizeof(Vector4);
	ambientDesc.Usage = D3D11_USAGE_DEFAULT;
	ambientDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ambientDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ambientData;
	ZeroMemory(&ambientData, sizeof(ambientData));
	ambientData.pSysMem = &ambient;

	ID3D11Buffer* ambientBuffer;
	mDevice->CreateBuffer(&ambientDesc, &ambientData, &ambientBuffer);
	mDeviceContext->PSSetConstantBuffers(ambientBufferSlot, 1, &ambientBuffer);
	ambientBuffer->Release();

	// PREPARE MATERIAL
	D3D11_BUFFER_DESC materialDesc;
	ZeroMemory(&materialDesc, sizeof(materialDesc));
	materialDesc.ByteWidth = sizeof(Material);
	materialDesc.Usage = D3D11_USAGE_DEFAULT;
	materialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialDesc.CPUAccessFlags = 0;

	mDevice->CreateBuffer(&materialDesc, NULL, &materialBuffer);
	mDeviceContext->PSSetConstantBuffers(materialBufferSlot, 1, &materialBuffer);

	// UPLOAD MVP MATRICES
	XMMATRIX modelMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX viewMatrix = XMMatrixLookAtRH(eye, center, up);
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(XMConvertToRadians(60.0f), 800 / 800, 1.0f, 500.0f);

	ID3D11Buffer* modelMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, modelMatrix);
	mDeviceContext->VSSetConstantBuffers(modelMatrixBufferSlot, 1, &modelMatrixBuffer);
	modelMatrixBuffer->Release();

	viewMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, viewMatrix);
	mDeviceContext->VSSetConstantBuffers(viewMatrixBufferSlot, 1, &viewMatrixBuffer);
	viewMatrixBuffer->Release();

	ID3D11Buffer* projectionMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, projectionMatrix);
	mDeviceContext->VSSetConstantBuffers(projectionMatrixBufferSlot, 1, &projectionMatrixBuffer);
	projectionMatrixBuffer->Release();

	return true;
}

void TestRasterizationDX::Update()
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

	XMMATRIX viewMatrix = XMMatrixLookAtRH(eye, center, up);
	mDeviceContext->UpdateSubresource(viewMatrixBuffer, 0, NULL, &viewMatrix, 0, 0);
}

void TestRasterizationDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11Buffer* buffers[2] = { vertexBuffer, instanceBuffer };
	UINT bufferStrides[2] = { sizeof(Vertex), 3 * sizeof(float) };
	UINT bufferOffsets[2] = { 0, 0 };
	mDeviceContext->IASetVertexBuffers(0, 2, buffers, bufferStrides, bufferOffsets);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->UpdateSubresource(materialBuffer, 0, NULL, &material, 0, 0);

	mDeviceContext->DrawInstanced(vertices.size(), offsets.size(), 0, 0);
}