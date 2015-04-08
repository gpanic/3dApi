#pragma once
#include "App3D.h"
#include <d3d11_2.h>

class DXApp : public App3D
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

protected:
	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mContext;
	IDXGISwapChain*				mSwapChain;
	ID3D11RenderTargetView*		mRenderTargetView;
	D3D_DRIVER_TYPE				mDriverType;
	D3D_FEATURE_LEVEL			mFeatureLevel;
	D3D11_VIEWPORT				mViewport;

	bool InitAPI() override;
	bool InitScene() override;
	void Update(float dt) override;
	void Render(float dt) override;
	void SwapBuffer() override;
};

