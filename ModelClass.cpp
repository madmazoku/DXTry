#include "ModelClass.h"

ModelClass::ModelClass(const ConfigClass& config) : m_config(config), m_pTexture(NULL), m_pVertexBuffer(NULL), m_pIndexBuffer(NULL), m_vertexCount(0), m_indexCount(0)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if (!InitializeBuffers(pDevice))
		return false;

	// Load the texture for this model.
	if (!LoadTexture(pDevice, pDeviceContext, "Wall"))
		return false;

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* pDeviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(pDeviceContext);

}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* pDevice)
{

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
//	VertexTypeColor* pVertices = new VertexTypeColor[m_vertexCount];
	//VertexTypeTex* pVertices = new VertexTypeTex[m_vertexCount];
	VertexTypeLight* pVertices = new VertexTypeLight[m_vertexCount];

	// Create the index array.
	unsigned long* pIndices = new unsigned long[m_indexCount];

// Load the vertex array with data. Part 1
	pVertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
//	pVertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pVertices[0].texture = XMFLOAT2(0.0f, 0.0f);
	pVertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	pVertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
//	pVertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pVertices[1].texture = XMFLOAT2(0.0f, 1.0f);
	pVertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	pVertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top right.
//	pVertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pVertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	pVertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	pVertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
//	pVertices[3].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	pVertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	pVertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

// Load the index array with data.
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 3;
	pIndices[3] = 3;
	pIndices[4] = 1;
	pIndices[5] = 2;

// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VertexTypeColor) * m_vertexCount;
	//vertexBufferDesc.ByteWidth = sizeof(VertexTypeTex) * m_vertexCount;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeLight) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] pVertices;
	delete[] pIndices;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = NULL;
		m_indexCount = 0;
	}

	// Release the vertex buffer.
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
		m_vertexCount = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* pDeviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
//	stride = sizeof(VertexTypeColor);
	//stride = sizeof(VertexTypeTex);
	stride = sizeof(VertexTypeLight);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& ddsFilename)
{
	// Create the texture object.
	m_pTexture = new TextureClass(m_config);

	// Initialize the texture object.
	if (!m_pTexture->Initialize(pDevice, pDeviceContext, ddsFilename))
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = NULL;
	}
}
