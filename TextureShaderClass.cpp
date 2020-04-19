#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass(const ConfigClass& config) : m_config(config), m_pVertexShader(NULL), m_pPixelShader(NULL), m_pLayout(NULL), m_pMatrixBuffer(NULL), m_pSampleState(NULL)
{
}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* pDevice)
{
	// Initialize the vertex and pixel shaders.
	if (!InitializeShader(pDevice, "Texture_VS", "Texture_PS"))
		return false;

	return true;
}

void TextureShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}

bool TextureShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture)
{
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix, pTexture))
		return false;

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, indexCount);

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device* pDevice, const std::string & vsShaderName, const std::string & psShaderName)
{
	ID3D10Blob* pErrorMessage = NULL;
	ID3D10Blob* pVertexShaderBuffer = NULL;
	ID3D10Blob* pPixelShaderBuffer = NULL;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Read compiled shader buffers
	D3DReadFileToBlob(m_config.pathToShader(vsShaderName).wstring().c_str(), &pVertexShaderBuffer);
	D3DReadFileToBlob(m_config.pathToShader(psShaderName).wstring().c_str(), &pPixelShaderBuffer);

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

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
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

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	if (FAILED(pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState)))
		return false;

	return true;
}

void TextureShaderClass::ShutdownShader()
{
	// Release the sampler state.
	if (m_pSampleState)
	{
		m_pSampleState->Release();
		m_pSampleState = NULL;
	}

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
		m_pVertexShader = 0;
	}
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture)
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

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	// Render the triangle.
//	pDeviceContext->DrawIndexed(indexCount, 0, 0);
	pDeviceContext->DrawIndexed(8, 0, 0);
	pDeviceContext->DrawIndexed(8, 0, indexCount);
}
