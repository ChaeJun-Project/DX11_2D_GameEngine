#include "stdafx.h"
#include "Light2D.h"

#include "RenderManager.h"

#include "GameObject.h"
#include "Transform.h"

Light2D::Light2D()
 :IComponent(ComponentType::Light2D)
{
   ZeroMemory(&m_light2D_data, sizeof(LightInfo));

   m_light2D_data.ligth_color.color = Vector4::White;
   m_light2D_data.ligth_color.specular = Vector4::White;
   m_light2D_data.ligth_color.ambient = Vector4::White;
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
	auto position = m_p_owner_game_object->GetComponent<Transform>()->GetTranslation();
	m_light2D_data.light_position = position;

	RenderManager::GetInstance()->RegisterLight2D(this , m_light2D_index);
}

//<summary>
//Light Position�� �� �����Ӹ��� �ش� ������Ʈ�� ������ GameObject�� Transform�� �����͸� �޾�
//�����ϱ� ������ Save & Load���� �������� ����
//</summary>
void Light2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	fprintf(p_file, "[Light Info]\n");
	
	//Light Color
	fprintf(p_file, "[Color]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.color, p_file);
	fprintf(p_file, "[Specular]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.specular, p_file);
	fprintf(p_file, "[Ambient]\n");
	FILE_MANAGER->FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.ambient, p_file);

	//Light Type
	fprintf(p_file, "[Type]\n");
	auto light_type = static_cast<UINT>(m_light2D_data.light_type);
	fprintf(p_file, "%d\n", light_type);

	//Light Direction
	fprintf(p_file, "[Direction]\n");
	FILE_MANAGER->FPrintf_Vector3(m_light2D_data.light_direction, p_file);

	//Light Ragne
	fprintf(p_file, "[Range]\n");
	fprintf(p_file, "%f\n", m_light2D_data.light_range);

	//Light Angle
	fprintf(p_file, "[Angle]\n");
	fprintf(p_file, "%f\n", m_light2D_data.light_angle);
}

void Light2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	FILE_MANAGER->FScanf(char_buffer, p_file);

	//Light Color
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.color, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.specular, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.ambient, p_file);

	//Light Type
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int light_type = -1;
	fscanf_s(p_file, "%d\n", &light_type);
	m_light2D_data.light_type = static_cast<LightType>(light_type);

	//Light Direction
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector3(m_light2D_data.light_direction, p_file);

	//Light Ragne
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_light2D_data.light_range);

	//Light Angle
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_light2D_data.light_angle);
}