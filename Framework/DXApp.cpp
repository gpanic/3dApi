#include "DXApp.h"

DXApp::DXApp(HINSTANCE hInstance) : App3D(hInstance)
{
	mAppTitle = "DirectX App";
	mWindowClass = "DXAPPWNDCLASS";

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

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);

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