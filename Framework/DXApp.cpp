#include "DXApp.h"

DXApp::DXApp(HINSTANCE hInstance) : App3D(hInstance)
{
	mAppTitle = "DirectX App";
	mWindowClass = "DXAPPWNDCLASS";
	shaderPath = assetPath + "HLSL/";
	mDeviceInfoFileName = "dx_device_info";

	mDevice = nullptr;
	mDeviceContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	debug = false;
}

DXApp::~DXApp()
{
	mDeviceContext->Release();
	mSwapChain->Release();
	mDevice->Release();
	mDepthStencilView->Release();
	mRenderTargetView->Release();
}

bool DXApp::InitAPI()
{
	UINT createFlags = (debug) ? D3D11_CREATE_DEVICE_DEBUG : 0;
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	ID3D11Device* mDeviceOld = nullptr;
	ID3D11DeviceContext* mDeviceContextOld = nullptr;

	HRESULT result;
	for (unsigned int i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDevice(NULL, driverTypes[i], NULL, createFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &mDeviceOld, &mFeatureLevel, &mDeviceContextOld);

		if (result == E_INVALIDARG)
		{
			result = D3D11CreateDevice(NULL, driverTypes[i], NULL, createFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &mDeviceOld, &mFeatureLevel, &mDeviceContextOld);
		}

		if (SUCCEEDED(result))
		{
			mDriverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating D3D11 device", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	result = mDeviceOld->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mDevice));
	mDeviceContextOld->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mDeviceContext));

	IDXGIDevice1* dxgiDevice = nullptr;
	mDevice->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	IDXGIFactory2* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));

	DXGI_SWAP_CHAIN_DESC1 sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Width = mWidth;
	sd.Height = mHeight;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;

	dxgiFactory->CreateSwapChainForHwnd(mDevice, mWindow, &sd, NULL, NULL, &mSwapChain);

	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;
	mDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	mDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &mDepthStencilView);

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mViewport.Width = (FLOAT)mWidth;
	mViewport.Height = (FLOAT)mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &mViewport);

	mDeviceOld->Release();
	mDeviceContextOld->Release();
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
	backBuffer->Release();
	depthStencilBuffer->Release();

	return true;
}

void DXApp::UpdateWindowTitle()
{
	std::stringstream ss;
	ss << mAppTitle << " | FPS " << mFPS;
	SetWindowText(mWindow, ss.str().c_str());
}

void DXApp::SwapBuffer()
{
	mSwapChain->Present(0, 0);
}

void DXApp::SaveSnapshot(std::string filePath)
{
	ID3D11Texture2D *backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	backBuffer->GetDesc(&desc);
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	ID3D11Texture2D *newTex;
	mDevice->CreateTexture2D(&desc, NULL, &newTex);
	mDeviceContext->CopyResource(newTex, backBuffer);

	D3D11_MAPPED_SUBRESOURCE res;
	ZeroMemory(&res, sizeof(res));
	mDeviceContext->Map(newTex, 0, D3D11_MAP_READ, 0, &res);

	byte *bmpTempBuffer = reinterpret_cast<byte *>(res.pData);
	if (!bmpTempBuffer) return;

	byte *bmpBuffer = new byte[mWidth * mHeight * 3];

	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int i = (x + (y * mWidth)) * 3;
			int iInv = (x + ((mWidth - y - 1) * mWidth)) * 3;
			int c = iInv / 3;
			bmpBuffer[i + 0] = bmpTempBuffer[iInv + 2 + c];
			bmpBuffer[i + 1] = bmpTempBuffer[iInv + 1 + c];
			bmpBuffer[i + 2] = bmpTempBuffer[iInv + 0 + c];
		}
	}

	mDeviceContext->Unmap(newTex, 0);
	bmpTempBuffer = nullptr;
	backBuffer->Release();
	newTex->Release();

	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	if (!file.is_open()) return;

	BITMAPFILEHEADER bitmapFileHeader;
	bitmapFileHeader.bfType = 0x4D42; //"BM"
	bitmapFileHeader.bfSize = mWidth * mHeight * 3;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bitmapInfoHeader;
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = mWidth - 1;
	bitmapInfoHeader.biHeight = mHeight - 1;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = 0;
	bitmapInfoHeader.biXPelsPerMeter = 0; // ?
	bitmapInfoHeader.biYPelsPerMeter = 0; // ?
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;

	file.write(reinterpret_cast<char *>(&bitmapFileHeader), sizeof(bitmapFileHeader));
	file.write(reinterpret_cast<char *>(&bitmapInfoHeader), sizeof(bitmapInfoHeader));
	file.write(reinterpret_cast<char *>(bmpBuffer), mWidth * mHeight * 3);
	file.close();

	delete[] bmpBuffer;
}

void DXApp::SaveDeviceInfo(std::string filePath)
{
}