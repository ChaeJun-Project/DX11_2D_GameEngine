#include "stdafx.h"
#include "ParticleUpdateShader.h"

#include "GraphicsManager.h"

#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "ComputeShader.h"

ParticleUpdateShader::ParticleUpdateShader()
{
    ZeroMemory(&m_particle_system_data, sizeof(CBuffer_Material));
}

ParticleUpdateShader::~ParticleUpdateShader()
{
	m_p_compute_shader.reset();

	m_p_particle_buffer.reset();
	m_p_particle_shared_buffer.reset();
}

void ParticleUpdateShader::Excute()
{
	BindPipeline();

	auto particle_max_count = m_p_particle_buffer->GetElementCount();

	m_particle_system_data.i_array[0] = particle_max_count;

	// 상수버퍼 업데이트
	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::Material);
	constant_buffer->SetConstantBufferData(&m_particle_system_data, sizeof(CBuffer_Material));
	constant_buffer->SetBufferBindStage(PipelineStage::CS);
	constant_buffer->BindPipeline();

	UINT thread_group_x_count = 0;

	if ((particle_max_count % m_p_compute_shader->GetGroupThreadXCount()) == 0)
		thread_group_x_count = particle_max_count / m_p_compute_shader->GetGroupThreadXCount();

	else
		thread_group_x_count = (particle_max_count / m_p_compute_shader->GetGroupThreadXCount()) + 1;

	m_p_compute_shader->Dispatch(thread_group_x_count, 1, 1);

	Clear();
}

void ParticleUpdateShader::BindPipeline()
{
	m_p_particle_buffer->BindPipelineRW(0);
	m_p_particle_shared_buffer->BindPipelineRW(1);
}

void ParticleUpdateShader::Clear()
{
	m_p_particle_buffer->Clear();
	m_p_particle_buffer = nullptr;
}

void ParticleUpdateShader::SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader)
{
	m_p_compute_shader = p_compute_shader;
	m_p_compute_shader->SetGroupThreadCount(100, 1, 1);
}
