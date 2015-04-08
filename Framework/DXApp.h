#pragma once
#include "App3D.h"
#include <d3d11_2.h>
#include <d3dcompiler.h>

class DXApp : public App3D
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

protected:
	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mDeviceContext;
	IDXGISwapChain*				mSwapChain;
	ID3D11RenderTargetView*		mRenderTargetView;
	D3D_DRIVER_TYPE				mDriverType;
	D3D_FEATURE_LEVEL			mFeatureLevel;
	D3D11_VIEWPORT				mViewport;


	virtual bool InitScene() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	bool InitAPI() override;
	void UpdateWindowTitle() override;
	void SwapBuffer() override;
};

