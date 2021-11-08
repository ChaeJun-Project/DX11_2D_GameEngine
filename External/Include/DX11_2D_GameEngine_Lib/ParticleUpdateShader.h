#pragma once
#include "ComputeShader.h"

class StructuredBuffer;
class ComputeShader;

class ParticleUpdateShader final
{
public:
	ParticleUpdateShader();
	~ParticleUpdateShader();

	void Excute();

private:
	void BindPipeline();
	void Clear();

public:
	void SetStructuredBuffer(const std::shared_ptr<StructuredBuffer>& p_structured_buffer)
	{
		m_p_structured_buffer = p_structured_buffer;
	}

	void SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader)
	{
		m_p_compute_shader = p_compute_shader;
	}

private:
	std::shared_ptr<StructuredBuffer> m_p_structured_buffer;

	std::shared_ptr<ComputeShader> m_p_compute_shader;

};

