#include "stdafx.h"
#include "Transform.h"

#include "SpriteRenderer.h"

Transform::Transform()
	:IComponent(ComponentType::Transform)
{

}

//Transform::Transform(const Transform& origin)
//	: IComponent(origin.GetComponentType())
//{
//	m_local_translation = origin.m_local_translation;
//	m_local_rotation = origin.m_local_rotation;
//	m_local_scale = origin.m_local_scale;
//	m_world_matrix = origin.m_world_matrix;
//	m_game_object_side_state = origin.m_game_object_side_state;
//}

void Transform::FinalUpdate()
{
	UpdateWorldMatrix();
}

void Transform::UpdateWorldMatrix()
{
	/*switch (m_game_object_side_state)
	{
	case GameObjectSideState::Right:
		SetRotation(Quaternion::Identity);
		break;
	case GameObjectSideState::Left:
		SetRotation(Quaternion::QuaternionFromEulerAngle(Vector3(0.0f, 180.0f, 0.0f)));
		break;
	}*/

	auto scale = Matrix::Scaling(m_local_scale * m_mesh_scale);
	auto rotation = Matrix::RotationQuaternion(m_local_rotation);
	auto translation = Matrix::Translation(m_local_translation);

	//SRT 연산
	//해당 transform을 소유한 오브젝트가 부모 오브젝트가 있다면
	//해당 월드 행렬은 부모 오브젝트 기준의 월드 행렬
	m_world_matrix = scale * rotation * translation;

	//부모 오브젝트의 transform이 있다면
	//현재 오브젝트의 월드 행렬에 부모 오브젝트의 월드 행렬을 곱함
	//부모 오브젝트 기준의 월드 행렬에 부모 오브젝트의 월드 행렬을 곱함
	if (m_p_owner_game_object->HasParent())
	{
		auto parent_world_matrix = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
		m_world_matrix = m_world_matrix * parent_world_matrix;
	}
}

void Transform::SetLocalTranslation(const Vector3& local_translation)
{
	if (m_local_translation == local_translation)
		return;

	m_local_translation = local_translation;
}

void Transform::SetLocalRotation(const Quaternion& local_rotation)
{
	if (m_local_rotation == local_rotation)
		return;

	m_local_rotation = local_rotation;
}

void Transform::SetLocalScale(const Vector3& local_scale)
{
	if (m_local_scale == local_scale)
		return;

	m_local_scale = local_scale;
}

void Transform::Translate(const Vector3& move)
{
	//해당 오브젝트가 부모 오브젝트를 가지고 있다면
	//부모 오브젝트의 월드 행렬의 역행렬을 곱한 값을 더함
	if (m_p_owner_game_object->HasParent())
	{
		auto parent_world_matrix = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
		SetLocalTranslation(m_local_translation + (move * parent_world_matrix.Inverse()));
	}

	else
		SetLocalTranslation(m_local_translation + move);
}

void Transform::SetTranslation(const Vector3& translation)
{
	if (GetTranslation() == translation)
		return;

	if (m_p_owner_game_object->HasParent())
	{
		auto parent_world_matrix = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
		SetLocalTranslation(translation * parent_world_matrix.Inverse());
	}

	else
		SetLocalTranslation(translation);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	if (GetRotation() == rotation)
		return;

	if (m_p_owner_game_object->HasParent())
	{
		auto parent_rotation = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->GetRotation();
		SetLocalRotation(rotation * parent_rotation.Inverse());
	}

	else
		SetLocalRotation(rotation);
}

void Transform::SetScale(const Vector3& scale)
{
	if (GetScale() == scale)
		return;

	if (m_p_owner_game_object->HasParent())
	{
		auto parent_scale = m_p_owner_game_object->GetParent()->GetComponent<Transform>()->GetScale();
		SetLocalScale(scale / parent_scale);
	}

	else
		SetLocalScale(scale);
}

const Vector3& Transform::GetWorldScale()
{
	auto world_scale = m_local_scale;
	
	if (m_p_owner_game_object->HasParent())
	{

	}

	return world_scale;
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

	fwrite(&m_local_translation, sizeof(Vector3), 1, p_file); //오브젝트의 위치 정보
	fwrite(&m_local_rotation, sizeof(Quaternion), 1, p_file); //오브젝트의 회전 값
	fwrite(&m_local_scale, sizeof(Vector3), 1, p_file);       //오브젝트의 스케일 값
}

void Transform::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	fread(&m_local_translation, sizeof(Vector3), 1, p_file); //오브젝트의 위치 정보
	fread(&m_local_rotation, sizeof(Quaternion), 1, p_file); //오브젝트의 회전 값
	fread(&m_local_scale, sizeof(Vector3), 1, p_file);       //오브젝트의 스케일 값
}
