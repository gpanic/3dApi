#include "TestSceneDX.h"

TestSceneDX::TestSceneDX(HINSTANCE hInstance) : DXApp(hInstance)
{
	mAppTitle = "DirectX Test Scene";
	mBenchmarkResultName = "dx_test_scene";
	bgColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
	bg[0] = bgColor.r;
	bg[1] = bgColor.g;
	bg[2] = bgColor.b;
	bg[3] = bgColor.a;
}

TestSceneDX::~TestSceneDX()
{
	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	shadowVertexShaderBuffer->Release();
	shadowPixelShaderBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	shadowVertexShader->Release();
	shadowPixelShader->Release();
	materialBuffer->Release();
	for (ModelDX model : models)
	{
		model.Release();
	}
	viewMatrixBuffer->Release();
	for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)
	{
		shadowMapResources[i]->Release();
	}
	shadowMapSamplerState->Release();
}

bool TestSceneDX::InitScene()
{
	XMStoreFloat4(&up, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	XMStoreFloat4(&eye, XMVectorSet(5.0f, 5.0f, 8.0f, 1.0f));
	XMStoreFloat4(&right, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	XMStoreFloat4(&center, XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));

	ID3D11RasterizerState1 *rasterizerState;
	D3D11_RASTERIZER_DESC1 rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;

	mDevice->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);
	mDeviceContext->RSSetState(rasterizerState);
	rasterizerState->Release();

	// COMPILE SHADERS
	D3DCompileFromFile(Util::s2ws(shaderPath + "TestSceneVert.hlsl").c_str(), NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &vertexShaderBuffer, NULL);
	D3DCompileFromFile(Util::s2ws(shaderPath + "TestSceneFrag.hlsl").c_str(), NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &pixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	mDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);

	D3DCompileFromFile(Util::s2ws(shaderPath + "ShadowMappingVert.hlsl").c_str(), NULL, NULL, "vertexShader", "vs_5_0", NULL, NULL, &shadowVertexShaderBuffer, NULL);
	D3DCompileFromFile(Util::s2ws(shaderPath + "ShadowMappingFrag.hlsl").c_str(), NULL, NULL, "pixelShader", "ps_5_0", NULL, NULL, &shadowPixelShaderBuffer, NULL);
	mDevice->CreateVertexShader(shadowVertexShaderBuffer->GetBufferPointer(), shadowVertexShaderBuffer->GetBufferSize(), NULL, &shadowVertexShader);
	mDevice->CreatePixelShader(shadowPixelShaderBuffer->GetBufferPointer(), shadowPixelShaderBuffer->GetBufferSize(), NULL, &shadowPixelShader);

	// PREPARE MODELS
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	vertexLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vertex().position), D3D11_INPUT_PER_VERTEX_DATA, 0 });

	XMMATRIX modelMatrix;

	modelMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	modelMatrix *= XMMatrixTranslation(2.0f, 0.0f, -2.0f);
	models.push_back(ModelDX(modelPath + "chair.bin", modelPath + "chair.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixTranslation(-2.0f, 1.0f, -2.0f);
	models.push_back(ModelDX(modelPath + "sphere_smooth.bin", modelPath + "sphere_smooth.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixScaling(3.0f, 3.0f, 3.0f);
	modelMatrix *= XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(100.0f));
	modelMatrix *= XMMatrixTranslation(-2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX(modelPath + "knife.bin", modelPath + "knife.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixTranslation(2.0f, 1.0f, 2.0f);
	models.push_back(ModelDX(modelPath + "monkey.bin", modelPath + "monkey.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	modelMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	models.push_back(ModelDX(modelPath + "plane.bin", modelPath + "plane.mtl", mDevice, vertexShaderBuffer, vertexLayout, modelMatrix, true));

	// PREPARE LIGHTING
	lighting.ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	Light light1;
	light1.position = Vector4(-5.0f, 5.0f, 5.0f, 0.0f);
	light1.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light1.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	Light light2;
	light2.position = Vector4(5.0f, 5.0f, 5.0f, 0.0f);
	light2.diffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	light2.specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	lighting.lights[0] = light1;
	lighting.lights[1] = light2;

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

	RenderShadowMaps();

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = 0.f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

	mDevice->CreateSamplerState(&samplerDesc, &shadowMapSamplerState);

	// PREPARE MATERIAL BUFFER
	D3D11_BUFFER_DESC materialDesc;
	ZeroMemory(&materialDesc, sizeof(materialDesc));
	materialDesc.ByteWidth = sizeof(Material);
	materialDesc.Usage = D3D11_USAGE_DEFAULT;
	materialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialDesc.CPUAccessFlags = 0;

	mDevice->CreateBuffer(&materialDesc, NULL, &materialBuffer);
	mDeviceContext->PSSetConstantBuffers(materialBufferSlot, 1, &materialBuffer);

	// PREPARE VIEW AND PROJECTION
	XMMATRIX viewMatrix = XMMatrixLookAtRH(XMLoadFloat4(&eye), XMLoadFloat4(&center), XMLoadFloat4(&up));
	viewMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, viewMatrix);
	mDeviceContext->VSSetConstantBuffers(viewMatrixBufferSlot, 1, &viewMatrixBuffer);

	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(XMConvertToRadians(60.0f), 800 / 800, 1.0f, 500.0f);
	ID3D11Buffer* projectionMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, projectionMatrix);
	mDeviceContext->VSSetConstantBuffers(projectionMatrixBufferSlot, 1, &projectionMatrixBuffer);
	projectionMatrixBuffer->Release();

	mDeviceContext->PSSetShaderResources(0, 2, &shadowMapResources[0]);
	mDeviceContext->PSSetSamplers(0, 1, &shadowMapSamplerState);

	return true;
}

void TestSceneDX::Update()
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
		input.Reset();
	}
}

void TestSceneDX::Render()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bg);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

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

void TestSceneDX::RenderShadowMaps()
{
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(shadowMapDesc));
	shadowMapDesc.Width = SHADOW_RESOLUTION;
	shadowMapDesc.Height = SHADOW_RESOLUTION;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.SampleDesc.Quality = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC shadowMapDsvDesc;
	ZeroMemory(&shadowMapDsvDesc, sizeof(shadowMapDsvDesc));
	shadowMapDsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	shadowMapDsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowMapDsvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shadowMapResourceDesc;
	ZeroMemory(&shadowMapResourceDesc, sizeof(shadowMapResourceDesc));
	shadowMapResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shadowMapResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowMapResourceDesc.Texture2D.MipLevels = 1;
	shadowMapResourceDesc.Texture2D.MostDetailedMip = 0;

	D3D11_VIEWPORT shadowMapViewport;
	shadowMapViewport.Width = (FLOAT)SHADOW_RESOLUTION;
	shadowMapViewport.Height = (FLOAT)SHADOW_RESOLUTION;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0;
	shadowMapViewport.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &shadowMapViewport);

	XMMATRIX shadowViewProjectionMatrices[NUMBER_OF_LIGHTS];
	for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)
	{
		ID3D11Texture2D* shadowMap;
		mDevice->CreateTexture2D(&shadowMapDesc, NULL, &shadowMap);

		ID3D11DepthStencilView* shadowMapDsv;
		mDevice->CreateDepthStencilView(shadowMap, &shadowMapDsvDesc, &shadowMapDsv);

		mDevice->CreateShaderResourceView(shadowMap, &shadowMapResourceDesc, &shadowMapResources[i]);

		mDeviceContext->OMSetRenderTargets(0, 0, shadowMapDsv);
		mDeviceContext->ClearDepthStencilView(shadowMapDsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

		XMVECTOR lightInvDir = XMVector3Normalize(XMVectorSet(lighting.lights[i].position.x, lighting.lights[i].position.y, lighting.lights[i].position.z, 1.0f));
		XMMATRIX shadowProjectionMatrix = XMMatrixOrthographicRH(20, 20, -10, 20);
		XMMATRIX shadowViewMatrix = XMMatrixLookAtRH(lightInvDir, XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 1));
		shadowViewProjectionMatrices[i] = shadowViewMatrix * shadowProjectionMatrix;

		ID3D11Buffer* shadowViewProjectionMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, shadowViewProjectionMatrices[i]);
		mDeviceContext->VSSetConstantBuffers(shadowViewProjectionMatrixBufferSlot, 1, &shadowViewProjectionMatrixBuffer);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		for (ModelDX model : models)
		{
			mDeviceContext->IASetVertexBuffers(0, 1, &model.vertexBuffer, &stride, &offset);
			mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mDeviceContext->IASetInputLayout(model.inputLayout);

			mDeviceContext->VSSetShader(shadowVertexShader, 0, 0);
			mDeviceContext->PSSetShader(shadowPixelShader, 0, 0);

			mDeviceContext->VSSetConstantBuffers(modelMatrixBufferSlot, 1, &model.modelMatrixBuffer);

			mDeviceContext->Draw(model.vertexCount, 0);
		}

		shadowMap->Release();
		shadowMapDsv->Release();
		shadowViewProjectionMatrixBuffer->Release();
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	mDeviceContext->RSSetViewports(1, &mViewport);

	D3D11_BUFFER_DESC matrixDesc;
	ZeroMemory(&matrixDesc, sizeof(matrixDesc));
	matrixDesc.ByteWidth = sizeof(XMMATRIX) * NUMBER_OF_LIGHTS;
	matrixDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA matrixData;
	ZeroMemory(&matrixData, sizeof(matrixData));
	matrixData.pSysMem = &shadowViewProjectionMatrices[0];

	ID3D11Buffer* matrixBuffer;
	mDevice->CreateBuffer(&matrixDesc, &matrixData, &matrixBuffer);

	mDeviceContext->VSSetConstantBuffers(shadowViewProjectionMatrixBufferSlot, 1, &matrixBuffer);
	matrixBuffer->Release();

	XMMATRIX biasMatrix = XMMatrixSet(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.5, 0.5, 0.0, 1.0
		);

	ID3D11Buffer* shadowBiasMatrixBuffer = DXUtil::CreateMatrixBuffer(mDevice, biasMatrix);
	mDeviceContext->VSSetConstantBuffers(shadowBiasMatrixBufferSlot, 1, &shadowBiasMatrixBuffer);
	shadowBiasMatrixBuffer->Release();
}