#include "GraphicsClass.h"

#include <iostream>

GraphicsClass::GraphicsClass(const ConfigClass& config) : m_config(config), m_pDirect3D(NULL), m_pColorShader(NULL), m_pModel(NULL), m_pCamera(NULL)
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

	// Create the model object.
	// Initialize the model object.
	m_pModel = new ModelClass(m_config);
	if (!m_pModel->Initialize(m_pDirect3D->GetDevice()))
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

	return true;
}


void GraphicsClass::Shutdown()
{
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

	return;
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
	static float x = 0;
	m_pCamera->SetRotation(0.0f, 0.0f, 0.0f + x);
	x += 0.1f;
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pDirect3D->GetWorldMatrix(worldMatrix);
	m_pDirect3D->GetProjectionMatrix(projectionMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Render(m_pDirect3D->GetDeviceContext());

	// Render the model using the color shader.
	if (!m_pColorShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
		return false;

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}
