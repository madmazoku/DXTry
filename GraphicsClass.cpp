#include "GraphicsClass.h"

#include <iostream>

GraphicsClass::GraphicsClass(const ConfigClass& config) : m_config(config), m_pStates(nullptr), m_pEffectFactory(nullptr), m_pCMOModel(nullptr), m_pDirect3D(NULL), m_pColorShader(NULL), m_pTextureShader(NULL), m_pLightShader(NULL), m_pLight(NULL), m_pModel(NULL), m_pCamera(NULL)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	m_pDirect3D = new D3DClass(m_config);

	// Initialize the Direct3D object.
	if (!m_pDirect3D->Initialize(screenWidth, screenHeight, hwnd, 1000.0f, 0.1f))
	{
		std::cerr << "[ERROR] Could not initialize Direct3D" << std::endl;
		return false;
	}

	// Create the camera object.
	// Set the initial position of the camera.
	m_pCamera = new CameraClass(m_config);
	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the light object.
	// Initialize the light object.
	m_pLight = new LightClass(m_config);
	m_pLight->SetDiffuseColor(1.0f, 1.0f, 0.75f, 1.0f);
	m_pLight->SetDirection(0.0f, 0.0f, 1.0f);

	m_pStates = std::make_unique<CommonStates>(m_pDirect3D->GetDevice());
	m_pEffectFactory = std::make_unique<EffectFactory>(m_pDirect3D->GetDevice());
	m_pCMOModel = Model::CreateFromCMO(m_pDirect3D->GetDevice(), m_config.pathToCMO("Cube").wstring().c_str(), *m_pEffectFactory, DirectX::ModelLoader_Clockwise);

	// Create the model object.
	// Initialize the model object.
	m_pModel = new ModelClass(m_config);
	if (!m_pModel->Initialize(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext()))
	{
		std::cerr << "[ERROR] Could not initialize the model object" << std::endl;
		return false;
	}

	// Create the color shader object.
	// Initialize the color shader object.
	m_pColorShader = new ColorShaderClass(m_config);
	if (!m_pColorShader->Initialize(m_pDirect3D->GetDevice(), hwnd))
	{
		std::cerr << "[ERROR] Could not initialize the color shader object" << std::endl;
		return false;
	}

	// Create the texture shader object.
	// Initialize the color shader object.
	m_pTextureShader = new TextureShaderClass(m_config);
	if (!m_pTextureShader->Initialize(m_pDirect3D->GetDevice()))
	{
		std::cerr << "[ERROR] Could not initialize the texture shader object" << std::endl;
		return false;
	}

	// Create the light shader object.
	// Initialize the color shader object.
	m_pLightShader = new LightShaderClass(m_config);
	if (!m_pLightShader->Initialize(m_pDirect3D->GetDevice()))
	{
		std::cerr << "[ERROR] Could not initialize the light shader object" << std::endl;
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light shader object.
	if (m_pLightShader)
	{
		m_pLightShader->Shutdown();
		delete m_pLightShader;
		m_pLightShader = NULL;
	}

	// Release the texture shader object.
	if (m_pTextureShader)
	{
		m_pTextureShader->Shutdown();
		delete m_pTextureShader;
		m_pTextureShader = NULL;
	}

	// Release the color shader object.
	if (m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = NULL;
	}

	// Release the model object.
	if (m_pModel)
	{
		m_pModel->Shutdown();
		delete m_pModel;
		m_pModel = NULL;
	}

	if (m_pCMOModel)
		m_pCMOModel = nullptr;

	if (m_pEffectFactory)
		m_pEffectFactory = nullptr;

	if (m_pStates)
		m_pStates = nullptr;

	// Release the light object.
	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}

	// Release the camera object.
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// Release the Direct3D object.
	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = NULL;
	}
}


bool GraphicsClass::Frame()
{
	// Render the graphics scene.
	if (!Render())
		return false;

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the buffers to begin the scene.
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pDirect3D->GetWorldMatrix(worldMatrix);
	m_pDirect3D->GetProjectionMatrix(projectionMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);

	static float angle = 0;
	angle += 0.01f;
//	worldMatrix = XMMatrixMultiply(XMMatrixRotationY(angle * 0.5f), XMMatrixMultiply(XMMatrixRotationX(angle), worldMatrix));
	worldMatrix = XMMatrixMultiply(XMMatrixRotationX(sin(angle) - 0.5f), worldMatrix);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(0.01f, 0.01f, 0.01f), worldMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Render(m_pDirect3D->GetDeviceContext());

/*
// Render the model using the color shader.
	if (!m_pColorShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
		return false;
*/

/*
// Render the model using the texture shader.
	if (!m_pTextureShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_pModel->GetTexture()))
		return false;
*/

// Render the model using the texture shader.
//	if (!m_pLightShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_pModel->GetTexture(), m_pLight->GetDirection(), m_pLight->GetDiffuseColor()))
//		return false;

	m_pCMOModel->UpdateEffects([&](IEffect* efx) {
		auto pLight = dynamic_cast<IEffectLights*>(efx);
		if (pLight)
		{
			XMVECTOR lightDir = XMVector3Rotate(m_pLight->GetDirection(), XMVECTOR{ 0.0f, sin(angle), 0.0f, 1.0f });
			pLight->SetLightDirection(0, lightDir);
			pLight->SetLightDiffuseColor(0, m_pLight->GetDiffuseColor());
			pLight->SetLightEnabled(1, false);
			pLight->SetLightEnabled(2, false);
		}
	});
	m_pCMOModel->Draw(m_pDirect3D->GetDeviceContext(), *m_pStates, worldMatrix, viewMatrix, projectionMatrix);

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}
