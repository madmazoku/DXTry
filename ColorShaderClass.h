#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "ConfigClass.h"

using namespace DirectX;

class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass(const ConfigClass& config);
	~ColorShaderClass();
	
	bool Initialize(ID3D11Device* pDevice, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	bool InitializeShader(ID3D11Device* pDevice, HWND hwnd, const std::string& vsFilename, const std::string& psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hwnd, LPCWCH shaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount);

private:
	const ConfigClass& m_config;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;

};

