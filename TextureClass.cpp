#include "TextureClass.h"

TextureClass::TextureClass(const ConfigClass& config) : m_config(config), m_pTexture(NULL), m_pTextureView(NULL)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string & ddsImageName)
{
	if(FAILED(CreateDDSTextureFromFile(pDevice, pDeviceContext, m_config.pathToDDS(ddsImageName).wstring().c_str(), &m_pTexture, &m_pTextureView)))
		return false;

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if (m_pTextureView)
	{
		m_pTextureView->Release();
		m_pTextureView = NULL;
	}

	// Release the texture.
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_pTextureView;
}