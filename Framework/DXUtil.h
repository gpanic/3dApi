#pragma once
#include <string>
#include <d3d11_2.h>
#include <DirectXMath.h>
using namespace DirectX;

struct DXUtil
{
	static void LoadDDS(ID3D11Device1* device, const std::string &imagepath, ID3D11ShaderResourceView** resource);
	static ID3D11Buffer* CreateMatrixBuffer(ID3D11Device1 *device, const XMMATRIX &matrix);
	static ID3D11Buffer* CreateEmptyMatrixBuffer(ID3D11Device1 *device);
};