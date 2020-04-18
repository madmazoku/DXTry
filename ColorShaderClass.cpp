#include "ColorShaderClass.h"

#include <iostream>

ColorShaderClass::ColorShaderClass(const ConfigClass& config) : m_config(config), m_pVertexShader(NULL), m_pPixelShader(NULL), m_pLayout(NULL), m_pMatrixBuffer(NULL)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, "Color_VS", "Color_PS"))
			return false;

	return true;
}

void ColorShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device* pDevice, HWND hwnd, const std::string& vsFilename, const std::string& psFilename)
{
	ID3D10Blob* pErrorMessage = NULL;
	ID3D10Blob* pVertexShaderBuffer = NULL;
	ID3D10Blob* pPixelShaderBuffer = NULL;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	/*
	// Compile the vertex shader code.
	if (FAILED(D3DCompileFromFile(vsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVertexShaderBuffer, &pErrorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (pErrorMessage)
			OutputShaderErrorMessage(pErrorMessage, hwnd, vsFilename);
		else
			std::cerr << "[ColorShaderClass::InitializeShader] Missing Vertex Shader File: " << vsFilename << std::endl;

		return false;
	}

	// Compile the pixel shader code.
	if (FAILED(D3DCompileFromFile(psFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPixelShaderBuffer, &pErrorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (pErrorMessage)
			OutputShaderErrorMessage(pErrorMessage, hwnd, psFilename);
		else
			std::cerr << "[ColorShaderClass::InitializeShader] Missing Pixel Shader File: " << psFilename << std::endl;

		return false;
	}
	*/

	// Read compiled shader buffers
	D3DReadFileToBlob((m_config.getAppDir() / (vsFilename + ".cso")).wstring().c_str(), &pVertexShaderBuffer);
	D3DReadFileToBlob((m_config.getAppDir() / (psFilename + ".cso")).wstring().c_str(), &pPixelShaderBuffer);

	// Create the vertex shader from the buffer.
	if (FAILED(pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader)))
		return false;

	// Create the pixel shader from the buffer.
	if (FAILED(pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
		return false;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	if (FAILED(pDevice->CreateInputLayout(polygonLayout, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &m_pLayout)))
		return false;

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	pVertexShaderBuffer->Release();
	pPixelShaderBuffer->Release();

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer)))
		return false;

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if (m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = NULL;
	}

	// Release the layout.
	if (m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = NULL;
	}

	// Release the pixel shader.
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}

	// Release the vertex shader.
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}

	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hwnd, LPCWCH shaderFilename)
{
	char* pCompileErrors = (char*)(pErrorMessage->GetBufferPointer());
	unsigned long long bufferSize = pErrorMessage->GetBufferSize();

	std::cerr << "Shader compilation error: " << shaderFilename << "\n";
	std::cerr.write(pCompileErrors, bufferSize);
	std::cerr << std::endl;

	// Release the error message.
	pErrorMessage->Release();

	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* pMatrixData;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// Get a pointer to the data in the constant buffer.
	pMatrixData = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	pMatrixData->world = worldMatrix;
	pMatrixData->view = viewMatrix;
	pMatrixData->projection = projectionMatrix;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Render the triangle.
	pDeviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
