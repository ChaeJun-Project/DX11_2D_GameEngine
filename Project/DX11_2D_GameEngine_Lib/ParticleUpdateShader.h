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
		m_p_particle_shared_buffer = p_particle_shared_buffer;
	}
	//Set Particle World Position
	void SetParticleTranslation(const Vector3& world_postion)
	{
		m_particle_system_data.v4_array[0] = world_postion;
	}
	//Set Particle Spawn
	void SetParticleSpawnRange(const Vector3& spawn_range)
	{
		m_particle_system_data.v4_array[1] = spawn_range;
	}
	//Set Particle Scale
	void SetParticleScale(const Vector3& start_scale, const Vector3& end_scale)
	{
		m_particle_system_data.v4_array[2] = start_scale;
		m_particle_system_data.v4_array[3] = end_scale;
	}
	//Set Particle Color
	void SetParticleColor(const Color4& start_color, const Color4& end_color)
	{
		m_particle_system_data.v4_array[4] = Vector4(start_color.r, start_color.g, start_color.b, start_color.a);
		m_particle_system_data.v4_array[5] = Vector4(end_color.r, end_color.g, end_color.b, end_color.a);
	}
	//Set Particle Speed
	void SetParticleSpeed(const float& min_speed, const float& max_speed)
	{
		m_particle_system_data.f_array[0] = min_speed;
		m_particle_system_data.f_array[1] = max_speed;
	}
	//Set Particle Life
	void SetParticleLife(const float& min_life, const float& max_life)
	{
		m_particle_system_data.f_array[2] = min_life;
		m_particle_system_data.f_array[3] = max_life;
	}

private:
	std::shared_ptr<ComputeShader> m_p_compute_shader;
	CBuffer_Material m_particle_system_data;

	std::shared_ptr<StructuredBuffer> m_p_particle_buffer = nullptr;		//각각의 파티클 오브젝트에 대한 정보
	std::shared_ptr<StructuredBuffer> m_p_particle_shared_buffer = nullptr; //모든 파티클 오브젝트들이 공유하는 정보
};

