#include "stdafx.h"
#include "Collider2D.h"

Collider2D::Collider2D()
	:IComponent(ComponentType::Collider2D)
{
	m_p_mesh = ResourceManager::GetInstance()->GetMesh(Vector2(100.0f, 100.0f));
	m_p_material = std::make_shared<Material>("Collider2D_Material");
	m_p_material->SetShader(ResourceManager::GetInstance()->GetShaderResource(ShaderResourceType::Collider2D));
}

Collider2D::Collider2D(const Collider2D& origin)
	: IComponent(origin.GetComponentType())
{
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

void Collider2D::OnCollisionEnter(std::shared_ptr<Collider2D>& other_collider)
{
	++this->m_collision_count;
}

void Collider2D::OnCollisionExit(std::shared_ptr<Collider2D>& other_collider)
{
	--this->m_collision_count;
}

void Collider2D::OnCollision(std::shared_ptr<Collider2D>& other_collider)
{
}
