#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "ConfigClass.h"

using namespace DirectX;

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass(const ConfigClass& config);
	~ModelClass();

	bool Initialize(ID3D11Device* pDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* pDeviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* pDevice);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);

private:
	const ConfigClass& m_config;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount;
	int m_indexCount;
};

