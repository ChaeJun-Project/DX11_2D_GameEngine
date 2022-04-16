#include "stdafx.h"
#include "RectTransform.h"

#include "ConstantBuffer.h"

#include "GameObject.h"
#include "Canvas.h"

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
	g_cbuffer_wvpmatrix.world_inverse = m_world_matrix.Inverse();

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}

void RectTransform::UpdateWorldMatrix()
{
	//Anchor 값이 변경된 경우
	if (m_p_owner_game_object->HasParent() && (m_pre_anchor != m_anchor))
	{
		UpdateAnchorPosition();
		m_pre_anchor = m_anchor;
	}

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

		auto p_parent_rect_trasnform = dynamic_cast<RectTransform*>(p_parent_transform);

		//Anchor Matrix
		UpdateAnchorMatrix(p_parent_rect_trasnform->GetWidgetSize());

		//Inheritance Matrix
		m_parent_origin_world_matrix = p_parent_rect_trasnform->GetOriginWorldMatrix();
		m_origin_world_matrix = m_origin_world_matrix * m_parent_origin_world_matrix;
		m_world_matrix = m_world_matrix * m_parent_origin_world_matrix;
	}
}

void RectTransform::UpdateAnchorPosition()
{
	auto p_parent_rect_transform = m_p_owner_game_object->GetParent()->GetComponent(ComponentType::Transform);
	if (p_parent_rect_transform == nullptr)
		return;

	Vector3 anchor_translation = Vector3::Zero;
	auto parent_widget_size = dynamic_cast<RectTransform*>(p_parent_rect_transform)->GetWidgetSize();

	//이전 Anchor 값과의 차이만큼 Local Translation에 값을 더함
	anchor_translation.x = (m_pre_anchor.x - m_anchor.x) * parent_widget_size.x;
	anchor_translation.y = (m_anchor.y - m_pre_anchor.y) * parent_widget_size.y;

	m_local_translation += anchor_translation;
}

void RectTransform::UpdateAnchorMatrix(const Vector2& widget_size)
{
	Vector3 anchor_translation = Vector3::Zero;

	//Anchor Matrix
	//Parent GameObject의 Rect Transform의 Widget Size를 기준으로 
	//현재 GameObject의 Rect Transform의 Anchor 값을 바탕으로 상대적인 변환을 위한 Matrix 생성
	anchor_translation.x = (m_anchor.x - 0.5f) * widget_size.x;
	anchor_translation.y = (0.5f - m_anchor.y) * widget_size.y;

	m_anchor_matrix = Matrix::Translation(anchor_translation);

	m_origin_world_matrix = m_origin_world_matrix * m_anchor_matrix;
	m_world_matrix = m_world_matrix * m_anchor_matrix;
}

void RectTransform::InitialzieProperty()
{
	__super::InitialzieProperty();

	m_anchor_matrix = Matrix::Identity;
}

void RectTransform::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Transform

	//Widget Size
	fprintf(p_file, "[Widget Size]\n");
	FILE_MANAGER->FPrintf_Vector2(m_widget_size, p_file);

	//Anchor
	fprintf(p_file, "[Anchor]\n");
	FILE_MANAGER->FPrintf_Vector2(m_anchor, p_file);
}

void RectTransform::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //Transform

	char char_buffer[256] = { 0 };

	//Widget Size
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_widget_size, p_file);

	//Anchor
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_anchor, p_file);

	m_pre_anchor = m_anchor;
}
