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

	virtual void Start() override;

	virtual void FinalUpdate() override;

	void Render();

private:
	void BindPipeline();

	void Clear();
	
public:
	std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
	std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }

public:
	//Set Particle Texture
	void SetParticleTexture(const std::shared_ptr<Texture>& p_particle_texture)
	{
		m_p_particle_texture = p_particle_texture;
	}
	//Set Compute Shader
	void SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader);
	//Set Particle Activable Count
	void SetParticleActivableCount(const UINT& activable_count)
	{
		m_activable_count = activable_count;
	}
	//Set Particle Count
	void SetMaxParticleCount(const UINT& max_count)
	{
		m_max_particle_count = max_count;
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
	void SetParticleColor(const Vector4& start_color, const Vector4& end_color)
	{
		m_start_color = start_color;  //���� �� ��ƼŬ ����
		m_end_color = end_color;	  //���� �� ��ƼŬ ����
	}
	//Set Particle Speed
	void SetParticleSpeed(const float& min_speed, const float& max_speed)
	{
		m_min_speed = min_speed;
		m_max_speed = max_speed;
	}
	//Set Particle Life
	void SetParticleLife(const float& min_life, const float& max_life)
	{
		m_min_life = min_life;
		m_max_life = max_life;
	}
	//Set Particle Draw Frequency
	void SetParticleDrawFrequency(const float& draw_frequency)
	{
		m_spawn_frequency = draw_frequency;
	}

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

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
	UINT m_activable_count = 0;    //�� ���� Ȱ��ȭ�� �� �ִ� ��ƼŬ ����
	UINT m_max_particle_count = 0; //��ƼŬ �ִ� ���� ����

	//Particle Spawn Range
	Vector3 m_spawn_range = Vector3::Zero;  //��ƼŬ ���� ����

	//Particle Scale
	Vector3 m_start_scale = Vector3::One;  //���� �� ��ƼŬ ũ��
	Vector3 m_end_scale = Vector3::One; //���� �� ��ƼŬ ũ��

	//Particle Color
	Vector4 m_start_color = Vector4::White;  //���� �� ��ƼŬ ����
	Vector4 m_end_color = Vector4::White;    //���� �� ��ƼŬ ����

	//Particle Speed
	float m_min_speed = 0.0f;
	float m_max_speed = 0.0f;

	//Particle Life
	float  m_min_life = 0.0f; //���� �� ���� �� �ִ� �ּ� ��ƼŬ ���� �ð�
	float  m_max_life = 0.0f; //���� �� ���� �� �ִ� �ִ� ��ƼŬ ���� �ð�

	//Particle Spawn Time
	float m_spawn_frequency = 0.0f; //��ƼŬ ���� �� �ð�
	float m_accumulate_time = 0.0f; //���� �ð�
};

