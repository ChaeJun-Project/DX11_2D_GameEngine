#include "stdafx.h"
#include "Light2D.h"

#include "RenderManager.h"

#include "GameObject.h"
#include "Transform.h"

Light2D::Light2D()
	:IComponent(ComponentType::Light2D)
{
	ZeroMemory(&m_light2D_data, sizeof(Light2D_Info));

	m_light2D_data.color = Vector4::White;
}

Light2D::Light2D(const Light2D& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	m_light2D_index = origin.m_light2D_index;

	m_light2D_data = origin.m_light2D_data;
}

void Light2D::FinalUpdate()
{
	if (m_p_owner_game_object->GetIsActive())
	{
		auto light2D_position = m_p_owner_game_object->GetComponent<Transform>()->GetTranslation();
		m_light2D_data.position = light2D_position;

		RENDER_MANAGER->RegisterLight2D(this, m_light2D_index);
	}
}

//<summary>
//Light Position은 매 프레임마다 해당 컴포넌트를 소유한 GameObject의 Transform의 데이터를 받아
//설정하기 때문에 Save & Load에는 포함하지 않음
//</summary>
void Light2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	fprintf(p_file, "[Light Info]\n");

	//Light Color
	fprintf(p_file, "[Color]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_light2D_data.color, p_file);

	//Light Type
	fprintf(p_file, "[Type]\n");
	auto light_type = static_cast<UINT>(m_light2D_data.type);
	fprintf(p_file, "%d\n", light_type);

	//Light Ragne
	fprintf(p_file, "[Range]\n");
	fprintf(p_file, "%f\n", m_light2D_data.range);
}

void Light2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	FILE_MANAGER->FScanf(char_buffer, p_file);

	//Light Color
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_light2D_data.color, p_file);

	//Light Type
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int light2D_type = -1;
	fscanf_s(p_file, "%d\n", &light2D_type);
	m_light2D_data.type = static_cast<Light2DType>(light2D_type);

	//Light Ragne
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_light2D_data.range);
}