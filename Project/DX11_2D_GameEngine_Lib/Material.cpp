#include "stdafx.h"
#include "Material.h"

Material::Material(const std::string resource_name)
	:IResource(ResourceType::Material, resource_name)
{
	ZeroMemory(&m_material_data, sizeof(CBuffer_Material));

	//텍스처는 총 8장까지만 들어가므로
	//Vector의 capacity를 8로 초기화 
	m_p_texture_vector.reserve(8);
	m_p_texture_vector.resize(8);
}

void Material::BindPipeline()
{
	m_p_shader->BindPipeline();

	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Material);
	constant_buffer->SetConstantBufferData(&m_material_data, sizeof(CBuffer_Material));
	constant_buffer->SetBufferBindStage(PipelineStage::Graphics_ALL);
	constant_buffer->BindPipeline();

	for (UINT i = 0; i < m_p_texture_vector.size(); ++i)
	{
		if (m_p_texture_vector[i] != nullptr)
		{
			m_p_texture_vector[i]->SetPipelineStage(PipelineStage::Graphics_ALL);
			m_p_texture_vector[i]->SetBindSlot(i);
			m_p_texture_vector[i]->BindPipeline();
		}
	}
}

void Material::SetConstantBufferData(const Material_Parameter& material_parameter, void* data, std::shared_ptr<Texture> texture)
{
	switch (material_parameter)
	{
	case Material_Parameter::INT_0:
	case Material_Parameter::INT_1:
	case Material_Parameter::INT_2:
	case Material_Parameter::INT_3:
		m_material_data.i_array[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::INT_0)] = *(reinterpret_cast<int*>(data));
		break;


	case Material_Parameter::FLOAT_0:
	case Material_Parameter::FLOAT_1:
	case Material_Parameter::FLOAT_2:
	case Material_Parameter::FLOAT_3:
		m_material_data.f_array[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::FLOAT_0)] = *(reinterpret_cast<float*>(data));
		break;


	case Material_Parameter::VEC2_0:
	case Material_Parameter::VEC2_1:
	case Material_Parameter::VEC2_2:
	case Material_Parameter::VEC2_3:
		m_material_data.v2_array[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::VEC2_0)] = *(reinterpret_cast<Vector2*>(data));
		break;


	case Material_Parameter::VEC4_0:
	case Material_Parameter::VEC4_1:
	case Material_Parameter::VEC4_2:
	case Material_Parameter::VEC4_3:
	case Material_Parameter::VEC4_4:
	case Material_Parameter::VEC4_5:
	case Material_Parameter::VEC4_6:
	case Material_Parameter::VEC4_7:
		m_material_data.v4_array[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::VEC4_0)] = *(reinterpret_cast<Vector4*>(data));
		break;


	case Material_Parameter::MAT_0:
	case Material_Parameter::MAT_1:
	case Material_Parameter::MAT_2:
	case Material_Parameter::MAT_3:
		m_material_data.mx_array[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::MAT_0)] = *(reinterpret_cast<Matrix*>(data));
		break;


	case Material_Parameter::TEX_0:
	case Material_Parameter::TEX_1:
	case Material_Parameter::TEX_2:
	case Material_Parameter::TEX_3:
	case Material_Parameter::TEX_4:
	case Material_Parameter::TEX_5:
	case Material_Parameter::TEX_6:
	case Material_Parameter::TEX_7:
		m_p_texture_vector[static_cast<UINT>(material_parameter) - static_cast<UINT>(Material_Parameter::TEX_0)] = texture;
		break;
	}
}
