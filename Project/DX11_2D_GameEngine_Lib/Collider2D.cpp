#include "stdafx.h"
#include "Collider2D.h"

Collider2D::Collider2D()
	:IComponent(ComponentType::Collider2D)
{
	m_p_mesh = ResourceManager::GetInstance()->GetMesh(MeshType::Rectangle);
	m_p_material = ResourceManager::GetInstance()->GetMaterial("Collider2D_Green");
}

Collider2D::Collider2D(const Collider2D& origin)
	: IComponent(origin.GetComponentType())
{
	m_offset_position = origin.m_offset_position;
	m_offset_scale = origin.m_offset_scale; //유니티에서는 Size

	m_collider_world_matrix = origin.m_collider_world_matrix;

	m_collision_count = origin.m_collision_count;

	m_p_mesh = origin.m_p_mesh;
	m_p_material = origin.m_p_material;
}

Collider2D::~Collider2D()
{
	m_p_mesh.reset();
	m_p_material.reset();
}

void Collider2D::FinalUpdate()
{
	UpdateColliderWorldMatrix();
}

void Collider2D::Render()
{
	UpdateConstantBuffer();

	m_p_material->BindPipeline();

	m_p_mesh->Render();
}

void Collider2D::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = m_collider_world_matrix;

	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}

void Collider2D::UpdateColliderWorldMatrix()
{
    auto transform = m_p_owner_game_object->GetComponent<Transform>();
	auto world_matrix = transform->GetWorldMatrix();
	auto world_scale = transform->GetScale();
	auto mesh_scale = transform->GetMeshScale();

	auto default_scale = Vector3(m_default_size.x / mesh_scale.x, m_default_size.y/ mesh_scale.y, m_default_size.z / mesh_scale.z); //m_offset_scale이 (1.0f, 1.0f, 1.0f) 일 때
	auto scale = Matrix::Scaling(m_offset_scale * default_scale);
	auto translation = Matrix::Translation(m_offset_position);

	m_collider_world_matrix = scale * translation * world_matrix;
}

void Collider2D::OnCollisionEnter(Collider2D* other_collider)
{
	++m_collision_count;

	m_p_material = ResourceManager::GetInstance()->GetMaterial("Collider2D_Red");

	auto script = m_p_owner_game_object->GetComponent<Script>();
	if (script != nullptr)
		script->OnCollisionEnter(other_collider->GetGameObject());
}

void Collider2D::OnCollisionExit(Collider2D* other_collider)
{
	--m_collision_count;

	if (m_collision_count == 0)
		m_p_material = ResourceManager::GetInstance()->GetMaterial("Collider2D_Green");

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
