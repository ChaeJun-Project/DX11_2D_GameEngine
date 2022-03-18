#include "stdafx.h"
#include "RectTransform.h"

#include "ConstantBuffer.h"

#include "GameObject.h"
#include "Transform.h"

RectTransform::RectTransform()
	:IComponent(ComponentType::RectTransform)
{
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
	g_cbuffer_widget_wvpmatrix.widget_world = m_world_matrix;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::WidgetWVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_widget_wvpmatrix, sizeof(CBuffer_Widget_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS | PipelineStage::GS);
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
		auto p_parent_rect_transform = m_p_owner_game_object->GetParent()->GetComponent<RectTransform>();
		if (p_parent_rect_transform == nullptr)
			return;

		m_parent_origin_world_matrix = p_parent_rect_transform->m_origin_world_matrix;

		m_origin_world_matrix = m_origin_world_matrix * m_parent_origin_world_matrix;
		m_world_matrix = m_world_matrix * m_parent_origin_world_matrix;
	}
}

void RectTransform::SetTranslation(const Vector3& translation)
{
	if (GetTranslation() == translation)
		return;

	SetLocalTranslation(translation);
}

void RectTransform::SetLocalTranslation(const Vector3& local_translation)
{
	if (m_local_translation == local_translation)
		return;

	m_local_translation = local_translation;
}

void RectTransform::SetRotation(const Quaternion& rotation)
{
	if (GetRotation() == rotation)
		return;

	SetLocalRotation(rotation);
}

void RectTransform::SetLocalRotation(const Quaternion& local_rotation)
{
	if (m_local_rotation == local_rotation)
		return;

	m_local_rotation = local_rotation;
}

void RectTransform::SetScale(const Vector3& scale)
{
	if (GetScale() == scale)
		return;

	SetLocalScale(scale);
}

void RectTransform::SetLocalScale(const Vector3& local_scale)
{
	if (m_local_scale == local_scale)
		return;

	m_local_scale = local_scale;
}

void RectTransform::InitialzieProperty()
{
	m_local_translation = m_origin_world_matrix.GetTranslation();
	m_local_scale = m_origin_world_matrix.GetScale();
	m_local_rotation = m_origin_world_matrix.GetRotation();

	m_parent_origin_world_matrix = Matrix::Identity;
	m_origin_world_matrix = Matrix::Identity;
	m_world_matrix = Matrix::Identity;
}

void RectTransform::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Position
	fprintf(p_file, "[Position]\n");
	FILE_MANAGER->FPrintf_Vector3(m_local_translation, p_file);

	//Widget Size
	fprintf(p_file, "[Widget Size]\n");
	FILE_MANAGER->FPrintf_Vector2(m_widget_size, p_file);
	
	//Rotation
	fprintf(p_file, "[Rotation]\n");
	FILE_MANAGER->FPrintf_Vector4<Quaternion>(m_local_rotation, p_file);

	//Scale
	fprintf(p_file, "[Scale]\n");
	FILE_MANAGER->FPrintf_Vector3(m_local_scale, p_file);
}

void RectTransform::LoadFromScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Position
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector3(m_local_translation, p_file);

	//Widget Size
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_widget_size, p_file);

	//Rotation
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Quaternion>(m_local_rotation, p_file);

	//Scale
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector3(m_local_scale, p_file);
}
