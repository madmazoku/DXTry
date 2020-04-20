#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "ConfigClass.h"
#include "TextureClass.h"

using namespace DirectX;

class ModelClass
{
private:
	//struct VertexTypeColor
	//{
	//	XMFLOAT3 position;
	//	XMFLOAT4 color;
	//};
	//struct VertexTypeTex
	//{
	//	XMFLOAT3 position;
	//	XMFLOAT2 texture;
	//};
	struct VertexTypeLight
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	ModelClass(const ConfigClass& config);
	~ModelClass();

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Shutdown();
	void Render(ID3D11DeviceContext* pDeviceContext);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* pDevice);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);

	bool LoadTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& ddsFilename);
	void ReleaseTexture();

private:
	const ConfigClass& m_config;

	TextureClass* m_pTexture;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;
};

