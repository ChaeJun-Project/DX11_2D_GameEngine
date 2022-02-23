#include "stdafx.h"
#include "Collider2D.h"

#include "ConstantBuffer.h"

#include "Material.h"
#include "Mesh.h"

#include "GameObject.h"
#include "Transform.h"
#include "Script.h"

Collider2D::Collider2D()
	:IComponent(ComponentType::Collider2D)
{
	m_p_mesh = RESOURCE_MANAGER->GetResource<Mesh>("Rectangle_Mesh");
	auto clone_material = RESOURCE_MANAGER->GetResource<Material>("Collider2D_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);

	ChangeColliderBoxColorGreen();
}

Collider2D::Collider2D(const Collider2D& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

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

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
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

void Collider2D::ChangeColliderBoxColorGreen()
{
    if(m_p_material == nullptr)
	return;

	int flag = 0;
	m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &flag); //Red Option을 0으로 초기화
	flag = 1;
	m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &flag); //Green Option을 1로 초기화
}

void Collider2D::ChangeColliderBoxColorRed()
{
	if (m_p_material == nullptr)
		return;

	int flag = 0;
	m_p_material->SetConstantBufferData(Material_Parameter::INT_0, &flag); //Green Option을 0으로 초기화
	flag = 1;
	m_p_material->SetConstantBufferData(Material_Parameter::INT_1, &flag); //Red Option을 1로 초기화
}

void Collider2D::OnCollisionEnter(Collider2D* other_collider)
{
	++m_collision_count;

	ChangeColliderBoxColorRed();

	auto script = m_p_owner_game_object->GetComponent<Script>();
	if (script != nullptr)
		script->OnCollisionEnter(other_collider->GetGameObject());
}

void Collider2D::OnCollisionExit(Collider2D* other_collider)
{
	--m_collision_count;

	if (m_collision_count == 0)
		ChangeColliderBoxColorGreen();

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
	FILE_MANAGER->FPrintf_Vector2(m_offset_position, p_file);

	//Offset Scale
	fprintf(p_file, "[Offset Scale]\n");
	FILE_MANAGER->FPrintf_Vector2(m_offset_scale, p_file);
}

void Collider2D::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Offset Position
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_offset_position, p_file);

	//Offset Scale
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf_Vector2(m_offset_scale, p_file);
}
	
