#pragma once

#include <directxmath.h>

#include "ConfigClass.h"

using namespace DirectX;

class LightClass
{
public:
	LightClass(const ConfigClass& config);
	~LightClass();

	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

	XMVECTOR GetDiffuseColor();
	XMVECTOR GetDirection();

private:
	const ConfigClass& m_config;

	XMVECTOR m_diffuseColor;
	XMVECTOR m_direction;
};

