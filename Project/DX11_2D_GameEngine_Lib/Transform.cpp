#include "stdafx.h"
#include "Transform.h"


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
	auto scale = Matrix::Scaling(this->m_local_scale);
	auto rotation = Matrix::RotationQuaternion(this->m_local_rotation);
	auto translation = Matrix::Translation(this->m_local_translation);

	//SRT ����
	//�ش� transform�� ������ ������Ʈ�� �θ� ������Ʈ�� �ִٸ�
	//�ش� ���� ����� �θ� ������Ʈ ������ ���� ���
	this->m_world_matrix = scale * rotation * translation;

	//�θ� ������Ʈ�� transform�� �ִٸ�
	//���� ������Ʈ�� ���� ��Ŀ� �θ� ������Ʈ�� ���� ����� ����
	//�θ� ������Ʈ ������ ���� ��Ŀ� �θ� ������Ʈ�� ���� ����� ����
	if (m_p_owner_game_object.lock()->HasParent())
	{
	    auto parent_world_matrix = m_p_owner_game_object.lock()->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
		this->m_world_matrix = this->m_world_matrix * parent_world_matrix;
	}
}

void Transform::SetLocalTranslation(const Vector3& local_translation)
{
	if (this->m_local_translation == local_translation)
		return;

	this->m_local_translation = local_translation;
	UpdateWorldMatrix();
}

void Transform::SetLocalRotation(const Quaternion& local_rotation)
{
	if (this->m_local_rotation == local_rotation)
		return;

	this->m_local_rotation = local_rotation;
	UpdateWorldMatrix();
}

void Transform::SetLocalScale(const Vector3& local_scale)
{
	if (this->m_local_scale == local_scale)
		return;

	this->m_local_scale = local_scale;
	UpdateWorldMatrix();
}

void Transform::Translate(const Vector3& move)
{
	//�ش� ������Ʈ�� �θ� ������Ʈ�� ������ �ִٸ�
	//�θ� ������Ʈ�� ���� ����� ������� ���� ���� ����
	if (m_p_owner_game_object.lock()->HasParent())
	{ 
		auto parent_world_matrix = m_p_owner_game_object.lock()->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
		SetLocalTranslation(this->m_local_translation + (move * parent_world_matrix.Inverse()));
	}

	else
		SetLocalTranslation(this->m_local_translation + move);
}

void Transform::SetTranslation(const Vector3& translation)
{
	if (GetTranslation() == translation)
		return;

	auto parent_world_matrix = m_p_owner_game_object.lock()->GetParent()->GetComponent<Transform>()->GetWorldMatrix();
	SetLocalTranslation(m_p_owner_game_object.lock()->HasParent() ? translation * parent_world_matrix.Inverse() : translation);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	if (GetRotation() == rotation)
		return;

	auto parent_rotation = m_p_owner_game_object.lock()->GetParent()->GetComponent<Transform>()->GetRotation();
	SetLocalRotation(m_p_owner_game_object.lock()->HasParent() ? rotation * parent_rotation.Inverse() : rotation);
}

void Transform::SetScale(const Vector3& scale)
{
	if (GetScale() == scale)
		return;

	auto parent_scale = m_p_owner_game_object.lock()->GetParent()->GetComponent<Transform>()->GetScale();
	SetLocalScale(m_p_owner_game_object.lock()->HasParent() ? scale / parent_scale : scale);
}
 
void Transform::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = this->m_world_matrix;

	static auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}
