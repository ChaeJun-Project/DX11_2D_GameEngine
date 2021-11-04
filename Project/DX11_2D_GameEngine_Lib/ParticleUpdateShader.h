#pragma once
#include "ComputeShader.h"

class StructuredBuffer;

class ParticleUpdate final : public ComputeShader
{
public:
	ParticleUpdate();
	~ParticleUpdate();

	void Excute();

public:
	void BindPipeline();
	void Clear();

public:
	void SetStructuredBuffer(const std::shared_ptr<StructuredBuffer>& p_structured_buffer)
	{
		m_p_structured_buffer = p_structured_buffer;
	}

private:
	std::weak_ptr<StructuredBuffer> m_p_structured_buffer;


};

