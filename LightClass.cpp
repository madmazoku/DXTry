#include "LightClass.h"

LightClass::LightClass(const ConfigClass& config) : m_config(config)
{
	m_diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_direction = { 0.0f, 0.0f, 1.0f, 1.0f };
}


LightClass::~LightClass()
{
}


void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = { red, green, blue, alpha };
}


void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = { x, y, z, 1.0f };
}


XMVECTOR LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


XMVECTOR LightClass::GetDirection()
{
	return m_direction;
}