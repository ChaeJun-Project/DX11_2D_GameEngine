#include "stdafx.h"
#include "Light2D.h"

Light2D::Light2D()
 :IComponent(ComponentType::Light2D)
{
   ZeroMemory(&m_light2D_data, sizeof(CBuffer_Light2D));
}

Light2D::Light2D(const Light2D& origin)
	: IComponent(origin.GetComponentType())
{
	this->m_light2D_data = origin.m_light2D_data;
}

Light2D::~Light2D()
{
}

void Light2D::FinalUpdate()
{
}

void Light2D::UpdateConstantBuffer()
{
}
