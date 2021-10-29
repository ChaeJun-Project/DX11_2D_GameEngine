#include "stdafx.h"
#include "Light2D.h"

#include "Transform.h"

#include "RenderManager.h"

Light2D::Light2D()
 :IComponent(ComponentType::Light2D)
{
   ZeroMemory(&m_light2D_data, sizeof(LightInfo));
}

Light2D::Light2D(const Light2D& origin)
	: IComponent(origin.GetComponentType())
{
    this->m_light2D_index = origin.m_light2D_index;
	this->m_light2D_data = origin.m_light2D_data;
}

void Light2D::FinalUpdate()
{
	auto position = m_p_owner_game_object->GetComponent<Transform>()->GetTranslation();
	m_light2D_data.light_position = position;

	RenderManager::GetInstance()->RegisterLight2D(this , this->m_light2D_index);
}