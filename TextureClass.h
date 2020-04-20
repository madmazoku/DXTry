#pragma once

#include <d3d11.h>

#include "ConfigClass.h"

class TextureClass
{
public:
	TextureClass(const ConfigClass& config);
	~TextureClass();

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& ddsFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	const ConfigClass& m_config;

	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureView;
};

