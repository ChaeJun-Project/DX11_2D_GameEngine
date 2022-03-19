#include "stdafx.h"
#include "RectTransform.h"

#include "ConstantBuffer.h"

#include "GameObject.h"

RectTransform::RectTransform()
	:Transform(ComponentType::RectTransform)
{
}

RectTransform::RectTransform(Transform& transform)
	: Transform(ComponentType::RectTransform)
{
	m_local_translation = transform.GetLocalTranslation();
	m_local_rotation = transform.GetLocalRotation();
	m_local_scale = transform.GetLocalScale();

	m_parent_origin_world_matrix = transform.GetParentOriginWorldMatrix();
	m_origin_world_matrix = transform.GetOriginWorldMatrix();
	m_world_matrix = transform.GetWorldMatrix();
}

void RectTransform::FinalUpdate()
{
	UpdateWorldMatrix();
}

void RectTransform::operator=(const Transform& transform)
{
	m_local_translation = transform.GetLocalTranslation();
	m_local_rotation = transform.GetLocalRotation();
	m_local_scale = transform.GetLocalScale();

	m_parent_origin_world_matrix = transform.GetParentOriginWorldMatrix();
	m_origin_world_matrix = transform.GetOriginWorldMatrix();
	m_world_matrix = transform.GetWorldMatrix();
}

void RectTransform::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = m_world_matrix;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}

void RectTransform::UpdateWorldMatrix()
{
	auto scale_matrix = Matrix::Scaling(m_local_scale);
	auto rotation_matrix = Matrix::RotationQuaternion(m_local_rotation);
	auto translation_matrix = Matrix::Translation(m_local_translation);

	//Origin World Matrix
	m_origin_world_matrix = scale_matrix * rotation_matrix * translation_matrix;

	//World Matrix
	auto scale = m_local_scale;
	scale.x *= m_widget_size.x;
	scale.y *= m_widget_size.y;
	scale_matrix = Matrix::Scaling(scale);
	m_world_matrix = scale_matrix * rotation_matrix * translation_matrix;

	//부모 오브젝트에 Rect Transform이 있다면
	if (m_p_owner_game_object->HasParent())
	{
		auto p_parent_transform = m_p_owner_game_object->GetParent()->GetComponent(ComponentType::Transform);
		if (p_parent_transform == nullptr)
			return;

		m_parent_origin_world_matrix = dynamic_cast<Transform*>(p_parent_transform)->GetOriginWorldMatrix();

		m_origin_world_matrix = m_origin_world_matrix * m_parent_origin_world_matrix;
		m_world_matrix = m_world_matrix * m_parent_origin_world_matrix;
	}
}

void RectTransform::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Transform

	//Widget Size
	fprintf(p_file, "[Widget Size]\n");
	FILE_MANAGER->FPrintf_Vector2(m_widget_size, p_file);
}

void RectTransform::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //Transform

	char char_buffer[256] = { 0 };

	//Widget Size
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_widget_size, p_file);
}
