#include "stdafx.h"
#include "ParticleSystem.h"

#include "ResourceManager.h"

#include "ComputeShader.h"
#include "ParticleUpdateShader.h"
#include "StructuredBuffer.h"

ParticleSystem::ParticleSystem()
	:IComponent(ComponentType::ParticleSystem)
{
	m_p_mesh = ResourceManager::GetInstance()->GetMesh(100, 100);
	m_p_material = ResourceManager::GetInstance()->GetMaterial("Particle");

	m_p_particle_update_shader = std::make_shared<ParticleUpdateShader>();
	auto compute_shader = ResourceManager::GetInstance()->GetShaderResource(ShaderResourceType::Particle)->GetShader<ComputeShader>();
	m_p_particle_update_shader->SetComputeShader(compute_shader);

	m_p_structured_buffer = std::make_shared<StructuredBuffer>();
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
	m_p_structured_buffer.reset();
}

void ParticleSystem::Initialize()
{
	
}

void ParticleSystem::FinalUpdate()
{
	m_p_particle_update_shader->SetStructuredBuffer(m_p_structured_buffer);
	m_p_particle_update_shader->Excute();
}

void ParticleSystem::BindPipeline()
{
	m_p_structured_buffer->SetBufferBindStage(PipelineStage::VS | PipelineStage::GS |PipelineStage::PS);
	m_p_structured_buffer->SetBufferBindSlot(13);
	m_p_structured_buffer->BindPipeline();
}

void ParticleSystem::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	BindPipeline();

	m_p_material->BindPipeline();

	//인스턴싱을 사용하여 한 번에 렌더링
	m_p_mesh->RenderInstance(m_max_particle_count);
}
