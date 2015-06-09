#pragma once
#include "App3D.h"
#include "DXUtil.h"
#include <d3d11_2.h>
#include <d3dcompiler.h>

class DXApp : public App3D
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

	bool debug;

protected:
	
	ID3D11Device1*				mDevice;
	ID3D11DeviceContext1*		mDeviceContext;
	IDXGISwapChain1*			mSwapChain;
	ID3D11DepthStencilView*		mDepthStencilView;
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
	void SaveSnapshot(std::string file) override;
	void SaveDeviceInfo(std::string file) override;
};

