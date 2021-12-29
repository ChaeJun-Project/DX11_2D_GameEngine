#include "stdafx.h"
#include "ParticleSystem.h"

#include "TimeManager.h"
#include "ResourceManager.h"

#include "ComputeShader.h"
#include "ParticleUpdateShader.h"
#include "StructuredBuffer.h"

#include "Transform.h"

ParticleSystem::ParticleSystem()
	:IComponent(ComponentType::ParticleSystem)
{
	auto resource_manager = ResourceManager::GetInstance();

	m_p_mesh = resource_manager->GetMesh(MeshType::Point);
	auto material = resource_manager->GetMaterial("Default_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(material);

	//Create Paritcle Update Shader
	m_p_particle_update_shader = std::make_shared<ParticleUpdateShader>();

	//Create Particle Structured Buffer
	m_p_particle_buffer = std::make_shared<StructuredBuffer>();

	//Create Particle Shared Structured Buffer
	m_p_particle_shared_buffer = std::make_shared<StructuredBuffer>();
}

ParticleSystem::ParticleSystem(const ParticleSystem& origin)
	: IComponent(origin.GetComponentType())
{

}

ParticleSystem::~ParticleSystem()
{
	m_p_mesh.reset();
	m_p_material.reset();

	m_p_particle_update_shader.reset();
	m_p_particle_buffer.reset();
	m_p_particle_shared_buffer.reset();
}

void ParticleSystem::Start()
{
	m_p_particle_buffer->Create(sizeof(ParticleInfo), m_max_particle_count, SBufferType::Read_Write, false);

	m_p_particle_shared_buffer->Create(sizeof(ParticleShared), 1, SBufferType::Read_Write, true);
}

void ParticleSystem::FinalUpdate()
{
	//========================================================
	//m_p_particle_shared_buffer 업데이트
	//========================================================
	m_accumulate_time += TimeManager::GetInstance()->GetDeltaTime_float();

	//파티클 활성화 빈도 시간만큼 시간이 누적되었다면
	if (m_accumulate_time >= m_spawn_frequency)
	{
		m_accumulate_time = 0.0f;

		ParticleShared particle_shared;
		ZeroMemory(&particle_shared, sizeof(ParticleShared));

		particle_shared.activable_count = m_activable_count;
		m_p_particle_shared_buffer->SetStructuredBufferData(&particle_shared, sizeof(ParticleShared));
	}

	//========================================================
	//m_p_particle_buffer 업데이트
	//========================================================
	m_p_particle_update_shader->SetParticleSharedStructuredBuffer(m_p_particle_shared_buffer);
	m_p_particle_update_shader->SetParticleStructuredBuffer(m_p_particle_buffer);

	//Set Particle World Position
	auto object_world_position = m_p_owner_game_object->GetComponent<Transform>();
	m_p_particle_update_shader->SetParticleTranslation(object_world_position->GetTranslation());

	//Set Particle Spawn
	m_p_particle_update_shader->SetParticleSpawnRange(m_spawn_range);

	//Set Particle Scale
	m_p_particle_update_shader->SetParticleScale(m_start_scale, m_end_scale);

	//Set Particle Color
	m_p_particle_update_shader->SetParticleColor(m_start_color, m_end_color);

	//Set Particle Speed
	m_p_particle_update_shader->SetParticleSpeed(m_min_speed, m_max_speed);

	//Set Particle Life
	m_p_particle_update_shader->SetParticleLife(m_min_life, m_max_life);

	m_p_particle_update_shader->Excute();
}

void ParticleSystem::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	BindPipeline();

	m_p_material->BindPipeline();

	//인스턴싱을 사용하여 한 번에 렌더링
	m_p_mesh->RenderInstance(m_max_particle_count);

	Clear();
}

void ParticleSystem::BindPipeline()
{
	m_p_owner_game_object->GetComponent<Transform>()->UpdateConstantBuffer();

	m_p_particle_buffer->SetBufferBindStage(PipelineStage::VS | PipelineStage::GS | PipelineStage::PS);
	m_p_particle_buffer->SetBufferBindSlot(14);
	m_p_particle_buffer->BindPipeline();

	m_p_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_particle_texture);
}

void ParticleSystem::Clear()
{
	m_p_particle_buffer->Clear();
}

void ParticleSystem::SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader)
{
	m_p_particle_update_shader->SetComputeShader(p_compute_shader);
}
