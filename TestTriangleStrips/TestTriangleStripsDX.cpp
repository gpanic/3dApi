#include "TestTriangleStripsDX.h"

TestTriangleStripsDX::TestTriangleStripsDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Triangle Strips";
	mBenchmarkResultName = "dx_test_triangle_strips";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

TestTriangleStripsDX::~TestTriangleStripsDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	inputLayout->Release();
}

bool TestTriangleStripsDX::InitScene()
{
	XMStoreFloat4(&up, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	XMStoreFloat4(&eye, XMVectorSet(0.0f, 18.0f, 18.0f, 1.0f));
	XMStoreFloat4(&right, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	XMStoreFloat4(&center, XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));

	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;

	ID3D11RasterizerState1 *rasterizerState;
	D3D11_RASTERIZER_DESC1 rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;

	mDevice->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);
	mDeviceContext->RSSetState(rasterizerState);
	rasterizerState->Release();

	BinaryIO::ReadVector4s(binaryPath + "triangle_strip_plane.bin", vertices);

	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DCompileFromFile(Util::s2ws(shaderPath + "TestTriangleStripsVert.hlsl").c_str(), NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(Util::s2ws(shaderPath + "TestTriangleStripsFrag.hlsl").c_str(), NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = vertices.size() * sizeof(Vector4);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	mDevice->CreateInputLayout(vertexLayout, 1, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	// UPLOAD MVP MATRICES
	XMMATRIX modelMatrix = XMMatrixIdentity();
	XMMATRIX viewMatrix = XMMatrixLookAtRH(XMLoadFloat4(&eye), XMLoadFloat4(&center), XMLoadFloat4(&up));
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

void TestTriangleStripsDX::Update()
{
	if (processInput)
	{
		float rotAmount = 0.0f;
		XMMATRIX rotMatrix = XMMatrixIdentity();
		if (input.right || input.left)
		{
			if (input.right)
				rotAmount = rotDelta;
			if (input.left)
				rotAmount = -rotDelta;
			rotMatrix = XMMatrixRotationAxis(XMLoadFloat4(&up), XMConvertToRadians(rotAmount));
		}
		else if (input.up || input.down)
		{
			if (input.up)
				rotAmount = rotDelta;
			if (input.down)
				rotAmount = -rotDelta;
			rotMatrix = XMMatrixRotationAxis(XMLoadFloat4(&right), XMConvertToRadians(rotAmount));
		}

		XMStoreFloat4(&eye, XMVector3Transform(XMLoadFloat4(&eye), rotMatrix));
		XMStoreFloat4(&right, XMVector3Normalize(XMVector3Cross(XMLoadFloat4(&up), (XMLoadFloat4(&center) - XMLoadFloat4(&eye)))));

		XMMATRIX viewMatrix = XMMatrixLookAtRH(XMLoadFloat4(&eye), XMLoadFloat4(&center), XMLoadFloat4(&up));
		mDeviceContext->UpdateSubresource(viewMatrixBuffer, 0, NULL, &viewMatrix, 0, 0);
	}
}

void TestTriangleStripsDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	UINT stride = sizeof(Vector4);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mDeviceContext->IASetInputLayout(inputLayout);

	mDeviceContext->VSSetShader(vertexShader, 0, 0);
	mDeviceContext->PSSetShader(pixelShader, 0, 0);

	mDeviceContext->Draw(vertices.size(), 0);
}