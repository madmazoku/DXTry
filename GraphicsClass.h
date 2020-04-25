#pragma once

#include <Model.h>
#include <Effects.h>
#include <CommonStates.h>

#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
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

	std::unique_ptr<CommonStates> m_pStates;
	std::unique_ptr<EffectFactory> m_pEffectFactory;
	std::unique_ptr<Model> m_pCMOModel;

	D3DClass* m_pDirect3D;
	ColorShaderClass* m_pColorShader;
	TextureShaderClass* m_pTextureShader;
	LightShaderClass* m_pLightShader;
	LightClass* m_pLight;
	ModelClass* m_pModel;
	CameraClass* m_pCamera;
};

