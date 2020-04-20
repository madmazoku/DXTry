#pragma once

#include <directxmath.h>

#include "ConfigClass.h"

using namespace DirectX;

class CameraClass
{
public:
	CameraClass(const ConfigClass& config);
	~CameraClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX& viewMatrix);

private:
	const ConfigClass& m_config;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	XMMATRIX m_viewMatrix;
};

