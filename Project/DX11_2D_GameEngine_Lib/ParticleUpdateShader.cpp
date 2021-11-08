#include "stdafx.h"
#include "ParticleUpdateShader.h"

#include "StructuredBuffer.h"
#include "ComputeShader.h"

ParticleUpdateShader::ParticleUpdateShader()
{
}

ParticleUpdateShader::~ParticleUpdateShader()
{
	m_p_structured_buffer.reset();
	m_p_compute_shader.reset();
}

void ParticleUpdateShader::Excute()
{
	BindPipeline();

	m_p_compute_shader->Dispatch(32, 1, 1);

	Clear();
}

void ParticleUpdateShader::BindPipeline()
{
	m_p_structured_buffer->BindPipelineRW(0);
}

void ParticleUpdateShader::Clear()
{
	m_p_structured_buffer->Clear();
}
