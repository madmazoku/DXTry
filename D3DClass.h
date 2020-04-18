#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "ConfigClass.h"

using namespace DirectX;

class D3DClass
{
public:
	D3DClass(const ConfigClass& config);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX& projectionMatrix);
	void GetWorldMatrix(XMMATRIX& worldMatrix);
	void GetOrthoMatrix(XMMATRIX& orthoMatrix);

private:
	const ConfigClass& m_config;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

