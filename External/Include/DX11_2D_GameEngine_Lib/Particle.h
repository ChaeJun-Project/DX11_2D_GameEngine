#pragma once

#include "IResource.h"

struct ParticleProperty
{
	//Particle Position
	Vector3 postion = Vector3::Zero;
	
	//Particle Count
	UINT activable_count = 0;    //한 번에 활성화할 수 있는 파티클 개수
	UINT max_count = 0; //파티클 최대 생성 개수

	//Particle Spawn Range
	Vector3 spawn_range = Vector3::Zero;  //파티클 생성 범위

	//Particle Scale
	Vector3 start_scale = Vector3::One;  //생성 시 파티클 크기
	Vector3 end_scale = Vector3::One;	   //삭제 시 파티클 크기

	//Particle Color
	Vector4 start_color = Vector4::White;  //생성 시 파티클 색상
	Vector4 end_color = Vector4::White;    //삭제 시 파티클 색상

	//Particle Speed
	float min_speed = 0.0f;
	float max_speed = 0.0f;

	//Particle Life
	float  min_life = 0.0f; //생성 시 가질 수 있는 최소 파티클 생존 시간
	float  max_life = 0.0f; //생성 시 가질 수 있는 최대 파티클 생존 시간

	//Particle Spawn Frequency
	float spawn_frequency = 0.0f; //파티클 생성 빈도 시간
};

class StructuredBuffer;
class ComputeShader;

class Particle final : public IResource
{
public:
	Particle(const std::string& particle_resource_name);
	explicit Particle(const Particle& origin);
	~Particle();

	void Update();

private:
	void BindPipeline() override;
	void UpdateConstantBuffer();
	void Dispatch();
	
public:
	void CreateParticleBuffer();
	
public:
    //Set Compute Shader
	void SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader);

	//Set Particle Shared Structured Buffer
	void SetParticleSharedStructuredBuffer(const std::shared_ptr<StructuredBuffer>& p_particle_shared_buffer)
	{
		m_p_particle_shared_buffer = p_particle_shared_buffer;
	}

	//Particle Texture
	std::shared_ptr<Texture> GetParticleTexture() { SAFE_GET_POINTER(m_p_particle_texture); }
	void SetParticleTexture(const std::shared_ptr<Texture>& p_particle_texture)
	{
		m_p_particle_texture = p_particle_texture;
	}


public:
    //Set Particle Property
	//Particle World Position
	void SetParticleTranslation(const Vector3& world_postion)
	{
		m_particle_property.postion = world_postion;
		m_particle_property_data.v4_array[0] = m_particle_property.postion;
	}

	//Particle Count
	void SetParticleActivableCount(const UINT& activable_count)
	{
		m_particle_property.activable_count = activable_count;
	}
	void SetParticleMaxCount(const UINT& max_count)
	{
		m_particle_property.max_count = max_count;
	}

	//Set Particle Spawn
	void SetParticleSpawnRange(const Vector3& spawn_range)
	{
		m_particle_property.spawn_range = spawn_range;
		m_particle_property_data.v4_array[1] = spawn_range;
	}

	//Set Particle Scale
	void SetParticleStartScale(const Vector3& start_scale)
	{
		m_particle_property.start_scale = start_scale;
		m_particle_property_data.v4_array[2] = start_scale;
	}

	void SetParticleEndScale(const Vector3& end_scale)
	{
		m_particle_property.end_scale = end_scale;
		m_particle_property_data.v4_array[3] = end_scale;
	}

	//Set Particle Color
	void SetParticleStartColor(const Vector4& start_color)
	{
		m_particle_property.start_color = start_color;
		m_particle_property_data.v4_array[4] = start_color;
	}

	void SetParticleEndColor(const Vector4& end_color)
	{
		m_particle_property.end_color = end_color;
		m_particle_property_data.v4_array[5] = end_color;
	}

	//Set Particle Speed
	void SetParticleMinSpeed(const float& min_speed)
	{
		m_particle_property.min_speed = min_speed;
		m_particle_property_data.f_array[0] = min_speed;
	}

	void SetParticleMaxSpeed( const float& max_speed)
	{
		m_particle_property.max_speed = max_speed;
		m_particle_property_data.f_array[1] = max_speed;
	}

	//Set Particle Life
	void SetParticleMinLife(const float& min_life)
	{
		m_particle_property.min_life = min_life;
		m_particle_property_data.f_array[2] = min_life;
	}

	void SetParticleMaxLife(const float& max_life)
	{
		m_particle_property.max_life = max_life;
		m_particle_property_data.f_array[3] = max_life;
	}

	//Set Spawn Frequency
	void SetParticleSpawnFrequency(const float& spawn_frequency)
	{
		m_particle_property.spawn_frequency = spawn_frequency;
	}

public:
	bool SaveToFile(const std::string& tile_map_path) override;
	bool LoadFromFile(const std::string& tile_map_path) override;

public:
	CLONE(Particle);

private:
	std::shared_ptr<ComputeShader> m_p_compute_shader = nullptr;

	//Particle Shared
	std::shared_ptr<StructuredBuffer> m_p_particle_shared_buffer = nullptr;

	std::unique_ptr<StructuredBuffer> m_p_particle_buffer = nullptr; //각각의 파티클 오브젝트에 대한 정보
	
	std::shared_ptr<Texture> m_p_particle_texture = nullptr;
	
	//Particle Property
	ParticleProperty m_particle_property;
	CBuffer_Material m_particle_property_data;

	friend class ParticleRenderer;
};

