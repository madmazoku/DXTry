#pragma once

#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "ModelClass.h"
#include "CameraClass.h"
#include "ConfigClass.h"

class GraphicsClass
{
public:
	GraphicsClass(const ConfigClass& config);
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	const ConfigClass& m_config;

	D3DClass* m_pDirect3D;
	ColorShaderClass* m_pColorShader;
	ModelClass* m_pModel;
	CameraClass* m_pCamera;
};

