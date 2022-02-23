#include "stdafx.h"
#include "Transform.h"

#include "ConstantBuffer.h"

#include "SpriteRenderer.h"

#include "GameObject.h"

Transform::Transform()
	:IComponent(ComponentType::Transform)
{

}

void Transform::FinalUpdate()
{
	UpdateWorldMatrix();
}

void Transform::UpdateWorldMatrix()
{
	//SRT 연산
	//해당 transform을 소유한 오브젝트가 부모 오브젝트가 있다면
	//해당 월드 행렬은 부모 오브젝트 기준의 월드 행렬
	auto scale = Matrix::Scaling(m_local_scale);
	auto rotation = Matrix::RotationQuaternion(m_local_rotation);
	auto translation = Matrix::Translation(m_local_translation);

	//Origin World Matrix
	m_origin_world_matrix = scale * rotation * translation;

	scale = Matrix::Scaling(m_local_scale * m_mesh_scale);
	//World Matrix
	m_world_matrix = scale * rotation * translation;

	//부모 오브젝트의 transform이 있다면
	//현재 오브젝트의 월드 행렬에 부모 오브젝트의 월드 행렬을 곱함
	//부모 오브젝트 기준의 월드 행렬에 부모 오브젝트의 월드 행렬을 곱함
	if (m_p_owner_game_object->HasParent())
	{
		auto parent_origin_world_matrix = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->m_origin_world_matrix;

		m_origin_world_matrix = m_origin_world_matrix * parent_origin_world_matrix;
		m_world_matrix = m_world_matrix * parent_origin_world_matrix;
	}
}

void Transform::SetTranslation(const Vector3& translation)
{
	if (GetTranslation() == translation)
		return;

	SetLocalTranslation(translation);
}

void Transform::SetLocalTranslation(const Vector3& local_translation)
{
	if (m_local_translation == local_translation)
		return;

	m_local_translation = local_translation;
}

void Transform::SetRotation(const Quaternion& rotation)
{
	if (GetRotation() == rotation)
		return;

	SetLocalRotation(rotation);
}

void Transform::SetLocalRotation(const Quaternion& local_rotation)
{
	if (m_local_rotation == local_rotation)
		return;

	m_local_rotation = local_rotation;
}

void Transform::SetScale(const Vector3& scale)
{
	if (GetScale() == scale)
		return;

	SetLocalScale(scale);
}

void Transform::SetLocalScale(const Vector3& local_scale)
{
	if (m_local_scale == local_scale)
		return;

	m_local_scale = local_scale;
}

void Transform::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = m_world_matrix;

	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS | PipelineStage::GS);
	constant_buffer->BindPipeline();
}

void Transform::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Position
	fprintf(p_file, "[Position]\n");
	FILE_MANAGER->FPrintf_Vector3(m_local_translation, p_file);

	//Rotation
	fprintf(p_file, "[Rotation]\n");
	FILE_MANAGER->FPrintf_Vector4<Quaternion>(m_local_rotation, p_file);

	//Scale
	fprintf(p_file, "[Scale]\n");
	FILE_MANAGER->FPrintf_Vector3(m_local_scale, p_file);
}

void Transform::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Position
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector3(m_local_translation, p_file);

	//Rotation
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector4<Quaternion>(m_local_rotation, p_file);

	//Scale
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector3(m_local_scale, p_file);
}
