#include "stdafx.h"
#include "ParticleRenderer.h"

#include "ComputeShader.h"
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"

#include "Material.h"
#include "Mesh.h"
#include "Particle.h"

#include "GameObject.h"
#include "Transform.h"

ParticleRenderer::ParticleRenderer()
	:IComponent(ComponentType::ParticleRenderer)
{
	m_p_particle_shared_buffer = std::make_shared<StructuredBuffer>();
	m_p_particle_shared_buffer->Create(sizeof(ParticleShared), 1, SBufferType::Read_Write, true);

	m_p_mesh = RESOURCE_MANAGER->GetResource<Mesh>("Point_Mesh");
	auto clone_material = RESOURCE_MANAGER->GetResource<Material>("Default_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);
}

ParticleRenderer::ParticleRenderer(const ParticleRenderer& origin)
	: IComponent(origin.m_component_type)
{

}

ParticleRenderer::~ParticleRenderer()
{
	m_p_current_particle.reset();

	m_p_particle_shared_buffer.reset();

	m_p_mesh.reset();
	m_p_material.reset();
}

void ParticleRenderer::Start()
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->CreateParticleBuffer();
	m_p_current_particle->SetParticleSharedStructuredBuffer(m_p_particle_shared_buffer);
}

void ParticleRenderer::FinalUpdate()
{
	if (m_p_current_particle == nullptr)
		return;

	UpdateParticleShared();

	//Transform
	auto p_transform = m_p_owner_game_object->GetComponent<Transform>();
	
	m_p_current_particle->SetParticleTranslation(p_transform->GetLocalTranslation());

	m_p_current_particle->Update();
}

void ParticleRenderer::Render()
{
	if (m_p_current_particle == nullptr || m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	//Transform
	auto p_transform = m_p_owner_game_object->GetComponent<Transform>();
	p_transform->UpdateConstantBuffer();

	BindPipeline();

	auto p_particle_texture = m_p_current_particle->m_p_particle_texture;
	if (p_particle_texture != nullptr)
		m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, p_particle_texture);

	m_p_material->BindPipeline();

	//인스턴싱을 사용하여 한 번에 렌더링
	m_p_mesh->RenderInstance(m_p_current_particle->m_particle_property.max_count);
}

void ParticleRenderer::UpdateParticleShared()
{
	//Particle Shared
	m_accumulate_time += DELTA_TIME_F;

	//파티클 활성화 빈도 시간만큼 시간이 누적되었다면
	if (m_accumulate_time >= m_p_current_particle->m_particle_property.spawn_frequency)
	{
		m_accumulate_time = 0.0f;

		ParticleShared particle_shared;
		ZeroMemory(&particle_shared, sizeof(ParticleShared));

		particle_shared.activable_count = m_p_current_particle->m_particle_property.activable_count;
		m_p_particle_shared_buffer->SetStructuredBufferData(&particle_shared, sizeof(ParticleShared));
	}
}

void ParticleRenderer::BindPipeline()
{
	if (m_p_current_particle->m_p_particle_buffer == nullptr)
		return;

	//Geometry, Pixel Shader에 Particle Buffer(구조화 버퍼) 등록
	m_p_current_particle->m_p_particle_buffer->SetBufferBindStage(PipelineStage::GS | PipelineStage::PS);
	m_p_current_particle->m_p_particle_buffer->SetBufferBindSlot(14);
	m_p_current_particle->m_p_particle_buffer->BindPipeline();
}

void ParticleRenderer::SetCurrentParticle(const std::shared_ptr<Particle>& p_particle)
{
	if (p_particle == nullptr)
		return;

	m_p_current_particle = p_particle;
	m_p_current_particle->SetParticleSharedStructuredBuffer(m_p_particle_shared_buffer);
}

const std::shared_ptr<Texture> ParticleRenderer::GetCurrentParticleTexture()
{
	if (m_p_current_particle == nullptr)
		return nullptr;

	return m_p_current_particle->GetParticleTexture();
}

void ParticleRenderer::SetCurrentParticleTexture(const std::shared_ptr<Texture>& p_particle_texture)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleTexture(p_particle_texture);
}

void ParticleRenderer::SetMaterial(const std::shared_ptr<Material>& p_material)
{
	if (m_p_material != nullptr)
		m_p_material.reset();

	m_p_material = p_material;
}

void ParticleRenderer::SetMesh(const std::shared_ptr<Mesh>& p_mesh)
{
	if (m_p_mesh != nullptr)
		m_p_mesh.reset();

	m_p_mesh = p_mesh;
}

void ParticleRenderer::SetParticleActivableCount(const UINT& activable_count)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleActivableCount(activable_count);
}

const UINT ParticleRenderer::GetParticleActivableCount()
{
	if (m_p_current_particle == nullptr)
		return 0;

	return m_p_current_particle->m_particle_property.activable_count;
}

void ParticleRenderer::SetParticleMaxCount(const UINT& max_count)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleMaxCount(max_count);
}

const UINT ParticleRenderer::GetParticleMaxCount()
{
	if (m_p_current_particle == nullptr)
		return 0;

	return m_p_current_particle->m_particle_property.max_count;
}

void ParticleRenderer::SetParticleSpawnRange(const Vector3& spawn_range)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleSpawnRange(spawn_range);
}

const Vector3 ParticleRenderer::GetParticleSpawnRange()
{
	if (m_p_current_particle == nullptr)
		return Vector3::Zero;

	return m_p_current_particle->m_particle_property.spawn_range;
}

void ParticleRenderer::SetParticleStartScale(const Vector3& start_scale)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleStartScale(start_scale);
}

const Vector3 ParticleRenderer::GetParticleStartScale()
{
	if (m_p_current_particle == nullptr)
		return Vector3::Zero;

	return m_p_current_particle->m_particle_property.start_scale;
}

void ParticleRenderer::SetParticleEndScale(const Vector3& end_scale)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleEndScale(end_scale);
}

const Vector3 ParticleRenderer::GetParticleEndScale()
{
	if (m_p_current_particle == nullptr)
		return Vector3::Zero;

	return m_p_current_particle->m_particle_property.end_scale;
}

void ParticleRenderer::SetParticleStartColor(const Vector4& start_color)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleStartColor(start_color);
}

const Vector4 ParticleRenderer::GetParticleStartColor()
{
	if (m_p_current_particle == nullptr)
		return Vector4::Zero;

	return m_p_current_particle->m_particle_property.start_color;
}

void ParticleRenderer::SetParticleEndColor(const Vector4& end_color)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleEndColor(end_color);
}

const Vector4 ParticleRenderer::GetParticleEndColor()
{
	if (m_p_current_particle == nullptr)
		return Vector4::Zero;

	return m_p_current_particle->m_particle_property.end_color;
}

void ParticleRenderer::SetParticleMinSpeed(const float& min_speed)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleMinSpeed(min_speed);
}

const float ParticleRenderer::GetParticleMinSpeed()
{
	if (m_p_current_particle == nullptr)
		return 0.0f;

	return m_p_current_particle->m_particle_property.min_speed;
}

void ParticleRenderer::SetParticleMaxSpeed(const float& max_speed)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleMaxSpeed(max_speed);
}

const float ParticleRenderer::GetParticleMaxSpeed()
{
	if (m_p_current_particle == nullptr)
		return 0.0f;

	return m_p_current_particle->m_particle_property.max_speed;
}

void ParticleRenderer::SetParticleMinLife(const float& min_life)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleMinLife(min_life);
}

const float ParticleRenderer::GetParticleMinLife()
{
	if (m_p_current_particle == nullptr)
		return 0.0f;

	return m_p_current_particle->m_particle_property.min_life;
}

void ParticleRenderer::SetParticleMaxLife(const float& max_life)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleMaxLife(max_life);
}

const float ParticleRenderer::GetParticleMaxLife()
{
	if (m_p_current_particle == nullptr)
		return 0.0f;

	return m_p_current_particle->m_particle_property.max_life;
}

void ParticleRenderer::SetParticleSpawnFrequency(const float& spawn_frequency)
{
	if (m_p_current_particle == nullptr)
		return;

	m_p_current_particle->SetParticleSpawnFrequency(spawn_frequency);
}

const float ParticleRenderer::GetParticleSpawnFrequency()
{
	if (m_p_current_particle == nullptr)
		return 0.0f;

	return m_p_current_particle->m_particle_property.spawn_frequency;
}

void ParticleRenderer::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//Particle
	fprintf(p_file, "[Particle]\n");
	RESOURCE_MANAGER->SaveResource<Particle>(m_p_current_particle, p_file);

	//Material
	fprintf(p_file, "[Material]\n");
	RESOURCE_MANAGER->SaveResource<Material>(m_p_material, p_file);

	//Mesh
	fprintf(p_file, "[Mesh]\n");
	RESOURCE_MANAGER->SaveResource<Mesh>(m_p_mesh, p_file);
}

void ParticleRenderer::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//Particle
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Particle>(m_p_current_particle, p_file);
	//SetCurrentParticle(m_p_current_particle);

	//Material
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Material>(m_p_material, p_file);

	//Mesh
	FILE_MANAGER->FScanf(char_buffer, p_file);
	RESOURCE_MANAGER->LoadResource<Mesh>(m_p_mesh, p_file);
}
