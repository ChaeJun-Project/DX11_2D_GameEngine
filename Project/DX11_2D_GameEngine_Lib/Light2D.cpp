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
    m_light2D_index = origin.m_light2D_index;
	m_light2D_data = origin.m_light2D_data;
}

void Light2D::FinalUpdate()
{
	auto position = m_p_owner_game_object->GetComponent<Transform>()->GetTranslation();
	m_light2D_data.light_position = position;

	RenderManager::GetInstance()->RegisterLight2D(this , m_light2D_index);
}

void Light2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	fprintf(p_file, "[Light Info]\n");
	
	//Light Color
	fprintf(p_file, "[Color]\n");
	FileManager::FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.color, p_file);
	fprintf(p_file, "[Specular]\n");
	FileManager::FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.specular, p_file);
	fprintf(p_file, "[Ambient]\n");
	FileManager::FPrintf_Vector4<Vector4>(m_light2D_data.ligth_color.ambient, p_file);

	//Light Type
	fprintf(p_file, "[Type]\n");
	auto light_type = static_cast<UINT>(m_light2D_data.light_type);
	fprintf(p_file, "%d\n", light_type);

	//Light Direction
	fprintf(p_file, "[Direction]\n");
	FileManager::FPrintf_Vector3(m_light2D_data.light_direction, p_file);

	//Light Position
	fprintf(p_file, "[Position]\n");
	FileManager::FPrintf_Vector3(m_light2D_data.light_position, p_file);
	
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

	FileManager::FScanf(char_buffer, p_file);

	//Light Color
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.color, p_file);
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.specular, p_file);
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector4<Vector4>(m_light2D_data.ligth_color.ambient, p_file);

	//Light Type
	FileManager::FScanf(char_buffer, p_file);
	int light_type = -1;
	fscanf_s(p_file, "%d\n", &light_type);
	m_light2D_data.light_type = static_cast<LightType>(light_type);

	//Light Direction
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector3(m_light2D_data.light_direction, p_file);

	//Light Position
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector3(m_light2D_data.light_position, p_file);

	//Light Ragne
	FileManager::FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_light2D_data.light_range);

	//Light Angle
	FileManager::FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%f\n", &m_light2D_data.light_angle);
}