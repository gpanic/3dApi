#include "DXUtil.h"
#include <memory>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void DXUtil::LoadDDS(ID3D11Device1* device, const std::string &imagepath, ID3D11ShaderResourceView** resource)
{
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath.c_str(), "rb");
	if (fp == NULL){
		return;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	DXGI_FORMAT format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = DXGI_FORMAT_BC1_UNORM;
		break;
	case FOURCC_DXT3:
		format = DXGI_FORMAT_BC2_UNORM;
		break;
	case FOURCC_DXT5:
		format = DXGI_FORMAT_BC3_UNORM;
		break;
	default:
		free(buffer);
		return;
	}

	D3D11_SUBRESOURCE_DATA* texData = new D3D11_SUBRESOURCE_DATA[mipMapCount];

	unsigned int blockSize = (format == DXGI_FORMAT_BC1_UNORM) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */

	unsigned int currentWidth = width;
	unsigned int currentHeight = height;
	for (unsigned int level = 0; level < mipMapCount && (currentWidth || currentHeight); ++level)
	{
		unsigned int size = ((currentWidth + 3) / 4)*((currentHeight + 3) / 4)*blockSize;
		texData[level].pSysMem = buffer + offset;
		texData[level].SysMemPitch = ((currentWidth + 3) / 4)*blockSize;
		texData[level].SysMemSlicePitch = ((currentWidth + 3) / 4)*blockSize*((currentHeight + 3) / 4);

		offset += size;
		currentWidth /= 2;
		currentHeight /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (currentWidth < 1) currentWidth = 1;
		if (currentHeight < 1) currentHeight = 1;

	}

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = mipMapCount;
	texDesc.ArraySize = 1;
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	device->CreateTexture2D(&texDesc, texData, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
	resourceDesc.Format = format;
	resourceDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = mipMapCount;

	device->CreateShaderResourceView(texture, &resourceDesc, resource);

	delete[] texData;
	free(buffer);
}

ID3D11Buffer* DXUtil::CreateMatrixBuffer(ID3D11Device1 *device, const XMMATRIX &matrix)
{
	D3D11_BUFFER_DESC matrixDesc;
	ZeroMemory(&matrixDesc, sizeof(matrixDesc));
	matrixDesc.ByteWidth = sizeof(matrix);
	matrixDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA matrixData;
	ZeroMemory(&matrixData, sizeof(matrixData));
	matrixData.pSysMem = &matrix;

	ID3D11Buffer* matrixBuffer;
	device->CreateBuffer(&matrixDesc, &matrixData, &matrixBuffer);
	return matrixBuffer;
}

ID3D11Buffer* DXUtil::CreateEmptyMatrixBuffer(ID3D11Device1 *device)
{
	D3D11_BUFFER_DESC matrixDesc;
	ZeroMemory(&matrixDesc, sizeof(matrixDesc));
	matrixDesc.ByteWidth = sizeof(XMMATRIX);
	matrixDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.CPUAccessFlags = 0;

	ID3D11Buffer* matrixBuffer;
	device->CreateBuffer(&matrixDesc, NULL, &matrixBuffer);
	return matrixBuffer;
}