#include "DXApp.h"

DXApp::DXApp(HINSTANCE hInstance) : App3D(hInstance)
{
	mAppTitle = "DirectX App";
	mWindowClass = "DXAPPWNDCLASS";

	mDevice = nullptr;
	mDeviceContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;
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
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = mWidth;
	bufferDesc.Height = mHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = mWindow;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT result;
	for (unsigned int i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, NULL, featureLevels,
			numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, &mFeatureLevel, &mDeviceContext);

		if (SUCCEEDED(result))
		{
			mDriverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating device and swap chain", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
	backBuffer->Release();

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);

	mViewport.Width = (FLOAT)mWidth;
	mViewport.Height = (FLOAT)mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &mViewport);

	return true;
}

void DXApp::UpdateWindowTitle()
{
	stringstream ss;
	ss << mAppTitle << " | FPS " << mFPS;
	SetWindowText(mWindow, ss.str().c_str());
}

void DXApp::SwapBuffer()
{
	mSwapChain->Present(0, 0);
}