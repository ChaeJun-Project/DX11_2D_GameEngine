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
    //Set Compute Shader
	void SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader);
	//Set Particle Structured Buffer
	void SetParticleStructuredBuffer(const std::shared_ptr<StructuredBuffer>& p_particle_buffer)
	{
		m_p_particle_buffer = p_particle_buffer;
	}
	//Set Particle Shared Structured Buffer
	void SetParticleSharedStructuredBuffer(const std::shared_ptr<StructuredBuffer>& p_particle_shared_buffer)
	{
		m_p_particle_buffer = p_particle_shared_buffer;
	}
	//Set Particle World Position
	void SetParticleTranslation(const Vector3& world_postion)
	{
		m_particle_system_data.world_position = world_postion;
	}
	//Set Particle Spawn
	void SetParticleSpawnRange(const Vector3& spawn_range)
	{
		m_particle_system_data.spawn_range = spawn_range;
	}
	//Set Particle Scale
	void SetParticleScale(const Vector3& start_scale, const Vector3& end_scale)
	{
		m_particle_system_data.start_scale = start_scale;
		m_particle_system_data.end_scale = end_scale;
	}
	//Set Particle Color
	void SetParticleColor(const Color4& start_color, const Color4& end_color)
	{
		m_particle_system_data.start_color = start_color;  
		m_particle_system_data.end_color = end_color;	  
	}
	//Set Particle Speed
	void SetParticleSpeed(const float& speed)
	{
		m_particle_system_data.speed = speed;
	}
	//Set Particle Life
	void SetParticleLife(const float& min_life, const float& max_life)
	{
		m_particle_system_data.min_life = min_life;
		m_particle_system_data.max_life = max_life;
	}

private:
	std::shared_ptr<ComputeShader> m_p_compute_shader;
	CBuffer_Particle m_particle_system_data;

	std::shared_ptr<StructuredBuffer> m_p_particle_buffer = nullptr;		//각각의 파티클 오브젝트에 대한 정보
	std::shared_ptr<StructuredBuffer> m_p_particle_shared_buffer = nullptr; //모든 파티클 오브젝트들이 공유하는 정보
};

