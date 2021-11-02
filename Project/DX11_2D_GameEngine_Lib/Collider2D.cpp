#include "stdafx.h"
#include "Collider2D.h"

Collider2D::Collider2D()
	:IComponent(ComponentType::Collider2D)
{
	this->m_p_mesh = ResourceManager::GetInstance()->GetMesh(100, 100);
	this->m_p_material = ResourceManager::GetInstance()->GetMaterialResource("Collider2D_Green");
}

Collider2D::Collider2D(const Collider2D& origin)
	: IComponent(origin.GetComponentType())
{
	this->m_offset_position = origin.m_offset_position;
	this->m_offset_scale = origin.m_offset_scale; //유니티에서는 Size

	this->m_collider_world_matrix = origin.m_collider_world_matrix;

	this->m_collision_count = origin.m_collision_count;

	this->m_p_mesh = origin.m_p_mesh;
	this->m_p_material = origin.m_p_material;
}

Collider2D::~Collider2D()
{
	this->m_p_mesh.reset();
	this->m_p_material.reset();
}

void Collider2D::FinalUpdate()
{
	UpdateColliderWorldMatrix();
}

void Collider2D::Render()
{
	UpdateConstantBuffer();

	this->m_p_material->BindPipeline();

	this->m_p_mesh->Render();
}

void Collider2D::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = this->m_collider_world_matrix;

	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}

void Collider2D::UpdateColliderWorldMatrix()
{
	auto world_matrix = m_p_owner_game_object->GetComponent<Transform>()->GetWorldMatrix();

	auto scale = Matrix::Scaling(this->m_offset_scale);
	auto translation = Matrix::Translation(this->m_offset_position);

	this->m_collider_world_matrix = scale * translation * world_matrix;
}

void Collider2D::OnCollisionEnter(Collider2D* other_collider)
{
	++this->m_collision_count;

	this->m_p_material = ResourceManager::GetInstance()->GetMaterialResource("Collider2D_Red");

	auto script = m_p_owner_game_object->GetComponent<Script>();
	if (script != nullptr)
		script->OnCollisionEnter(other_collider->GetGameObject());
}

void Collider2D::OnCollisionExit(Collider2D* other_collider)
{
	--this->m_collision_count;

	if (this->m_collision_count == 0)
		this->m_p_material = ResourceManager::GetInstance()->GetMaterialResource("Collider2D_Green");

	auto script = m_p_owner_game_object->GetComponent<Script>();
	if (script != nullptr)
		script->OnCollisionEnter(other_collider->GetGameObject());
}

void Collider2D::OnCollision(Collider2D* other_collider)
{
	auto script = m_p_owner_game_object->GetComponent<Script>();
	if (script != nullptr)
		script->OnCollisionEnter(other_collider->GetGameObject());
}
