#pragma once
#include <string>
#include <d3d11_2.h>

struct DXUtil
{
	static void LoadDDS(ID3D11Device1* device, const std::string &imagepath, ID3D11ShaderResourceView** resource);
};