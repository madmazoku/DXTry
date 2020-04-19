#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;

#include "ConfigClass.h"

class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShaderClass(const ConfigClass& config);
	~TextureShaderClass();

	bool Initialize(ID3D11Device* pDevice);
	void Shutdown();
	bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture);

private:
	bool InitializeShader(ID3D11Device* pDevice, const std::string & vsShaderName, const std::string & psShaderName);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture);
	void RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount);

private:
	const ConfigClass& m_config;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;

	ID3D11SamplerState* m_pSampleState;
};

