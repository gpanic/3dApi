#pragma once
#include "DXApp.h"

class TestUniformUpdateDX : public DXApp
{
public:
	TestUniformUpdateDX(HINSTANCE hInstance);;
	virtual ~TestUniformUpdateDX();
private:
	bool InitScene() override;
	void Update() override;
	void Render() override;

	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;
	ID3D11InputLayout* inputLayout;
	float bg[4];

	ID3D11Buffer* colorsBlockBuffer;
};

