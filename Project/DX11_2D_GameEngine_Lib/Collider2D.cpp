#include "stdafx.h"
#include "Collider2D.h"

Collider2D::Collider2D()
	:IComponent(ComponentType::Collider2D)
{
	auto resource_manager = ResourceManager::GetInstance();

	m_p_mesh = resource_manager->GetMesh(MeshType::Rectangle);
	m_p_material = resource_manager->GetMaterial("Collider2D_Green");
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
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

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

	//Collider2D Box 크기 / Mesh 크기 => 해당 비율을 m_offset_position과 m_offset_scale 곱해줘야 100x100일 때의 offset 비율이 나옴
	//world_matrix에서 Mesh의 크기만큼 먼저 곱했기 때문에 이를 나눠주어야 함
	auto ratio_scale = Vector3(m_default_size.x / mesh_scale.x, m_default_size.y/ mesh_scale.y, 1.0f); 

	auto translation = Matrix::Translation(Vector3(m_offset_position.x, m_offset_position.y, 0.0f) * ratio_scale);
	auto scale = Matrix::Scaling(Vector3(m_offset_scale.x, m_offset_scale.y, 1.0f) * ratio_scale);

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

void Collider2D::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Offset Position
	fprintf(p_file, "[Offset Position]\n");
	FileManager::FPrintf_Vector2(m_offset_position, p_file);

	//Offset Scale
	fprintf(p_file, "[Offset Scale]\n");
	FileManager::FPrintf_Vector2(m_offset_scale, p_file);
}

void Collider2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Offset Position
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector2(m_offset_position, p_file);

	//Offset Scale
	FileManager::FScanf(char_buffer, p_file);
	FileManager::FScanf_Vector2(m_offset_scale, p_file);
}
	
