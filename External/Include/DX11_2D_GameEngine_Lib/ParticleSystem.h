#pragma once
#include "IComponent.h"

class Texture;
class Mesh;
class Material;

class StructuredBuffer;
class ParticleUpdateShader;

class ParticleSystem : public IComponent
{
public:
	ParticleSystem();
	explicit ParticleSystem(const ParticleSystem& origin);
	~ParticleSystem();

	virtual void FinalUpdate() override;

	void Render();

private:
	void BindPipeline();

	void Clear();

public:
	//Set Particle Texture
	void SetParticleTexture(const std::shared_ptr<Texture>& p_particle_texture)
	{
		m_p_particle_texture = p_particle_texture;
	}
	//Set Compute Shader
	void SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader);
	//Set Particle Count
	void SetMaxParticleCount(const UINT& max_particle_count)
	{
		m_max_particle_count = max_particle_count;
	}
	//Set Particle Spawn
	void SetParticleSpawnRange(const Vector3& spawn_range)
	{
		m_spawn_range = spawn_range;
	}
	//Set Particle Scale
	void SetParticleScale(const Vector3& start_scale, const Vector3& end_scale)
	{
		m_start_scale = start_scale;
		m_end_scale = end_scale;
	}
	//Set Particle Color
	void SetParticleColor(const Color4& start_color, const Color4& end_color)
	{
		m_start_color = start_color;  //���� �� ��ƼŬ ����
		m_end_color = end_color;	  //���� �� ��ƼŬ ����
	}
	//Set Particle Speed
	void SetParticleSpeed(const float& speed)
	{
		m_speed = speed;
	}
	//Set Particle Life
	void SetParticleLife(const float& min_life, const float& max_life)
	{
		m_min_life = min_life;
		m_max_life = max_life;
	}

public:
	CLONE(ParticleSystem);

private:
	std::shared_ptr<Mesh> m_p_mesh = nullptr;
	std::shared_ptr<Material> m_p_material = nullptr;

	std::shared_ptr<ParticleUpdateShader> m_p_particle_update_shader = nullptr;
	std::shared_ptr<StructuredBuffer> m_p_particle_buffer = nullptr;		//������ ��ƼŬ ������Ʈ�� ���� ����
	std::shared_ptr<StructuredBuffer> m_p_particle_shared_buffer = nullptr; //��� ��ƼŬ ������Ʈ���� �����ϴ� ����

    std::shared_ptr<Texture> m_p_particle_texture = nullptr;

	//��ƼŬ �ý��� ����
	//Particle Count
	UINT m_max_particle_count = 0; //��ƼŬ �ִ� ���� ����

	//Particle Spawn Range
	Vector3 m_spawn_range = Vector3::Zero;  //��ƼŬ ���� ����

	//Particle Scale
	Vector3 m_start_scale = Vector3::One;  //���� �� ��ƼŬ ũ��
	Vector3 m_end_scale = Vector3::One; //���� �� ��ƼŬ ũ��

	//Particle Color
	Color4 m_start_color = Color4::White;  //���� �� ��ƼŬ ����
	Color4 m_end_color = Color4::White;    //���� �� ��ƼŬ ����

	//Particle Speed
	float m_speed = 0.0f;

	//Particle Life
	float  m_min_life = 0.0f; //���� �� ���� �� �ִ� �ּ� ��ƼŬ ���� �ð�
	float  m_max_life = 0.0f; //���� �� ���� �� �ִ� �ִ� ��ƼŬ ���� �ð�

	//Particle Spawn Time
	float m_spawn_frequency = 0.0f; //��ƼŬ ���� �� �ð�
	float m_accumulate_time = 0.0f; //���� �ð�

};

