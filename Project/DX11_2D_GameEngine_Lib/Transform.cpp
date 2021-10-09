#include "stdafx.h"
#include "Transform.h"


Transform::Transform(GameObject* p_game_object)
	:IComponent(ComponentType::Transform, p_game_object)
{

}

Transform::~Transform()
{
	m_p_game_object.reset();
	m_child_transform_vector.clear();
	m_child_transform_vector.shrink_to_fit();
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
	this->m_world_matrix = scale * rotation * translation;

	//�θ� ������Ʈ�� transform�� �ִٸ�
	//���� ������Ʈ�� ���� ��Ŀ� �θ� ������Ʈ�� ���� ����� ����
	if (HasParent())
		this->m_world_matrix = this->m_world_matrix * this->m_p_parent_transform->GetWorldMatrix();

	//�ڽ� ������Ʈ�� ���� ��� ������Ʈ
	for (const auto& child_transform : this->m_child_transform_vector)
		child_transform->UpdateWorldMatrix();
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
	//�θ� ������Ʈ�� transform�� �ִٸ�
	//�θ� ������Ʈ�� ���� ����� ������� ���� ���� ����
	if (HasParent())
		SetLocalTranslation(this->m_local_translation + (move * this->m_p_parent_transform->GetWorldMatrix().Inverse()));

	else
		SetLocalTranslation(this->m_local_translation + move);
}

void Transform::SetTranslation(const Vector3& translation)
{
	if (GetTranslation() == translation)
		return;

	SetLocalTranslation(HasParent() ? translation * this->m_p_parent_transform->GetWorldMatrix().Inverse() : translation);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	if (GetRotation() == rotation)
		return;

	SetLocalRotation(HasParent() ? rotation * this->m_p_parent_transform->GetRotation().Inverse() : rotation);
}

void Transform::SetScale(const Vector3& scale)
{
	if (GetScale() == scale)
		return;

	SetLocalScale(HasParent() ? scale / this->m_p_parent_transform->GetScale() : scale);
}

void Transform::SetParent(Transform* new_parent)
{
	//TODO
}

Transform* Transform::GetChildFromIndex(const UINT& index)
{
	//TODO
	return nullptr;
}

Transform* Transform::GetChildFromObjectName(const std::string& object_name)
{
	//TODO
	return nullptr;
}

void Transform::AddChild(Transform* child)
{
	//TODO
}

void Transform::DetachChild()
{
	//TODO
}

void Transform::TachChild()
{
	//TODO
}

void Transform::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = this->m_world_matrix;

	static auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}
