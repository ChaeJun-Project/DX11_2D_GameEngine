#pragma once
#include "IComponent.h"

class Texture;
class Mesh;
class Material;

class Particle;
class StructuredBuffer;

class ParticleRenderer : public IComponent
{
public:
	ParticleRenderer();
	explicit ParticleRenderer(const ParticleRenderer& origin);
	~ParticleRenderer();

	void Start() override;
	void FinalUpdate() override;
	void Render();

private:
    void UpdateParticleShared();
	void BindPipeline();

public:
	//Particle
	const std::shared_ptr<Particle> GetCurrentParticle() { SAFE_GET_POINTER(m_p_current_particle); }
	void SetCurrentParticle(const std::shared_ptr<Particle>& p_particle);

	//Particle Texture
	const std::shared_ptr<Texture> GetCurrentParticleTexture();
	void SetCurrentParticleTexture(const std::shared_ptr<Texture>& p_particle_texture);

	//Material
	const std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }
	void SetMaterial(const std::shared_ptr<Material>& p_material);

	//Mesh
	const std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
	void SetMesh(const std::shared_ptr<Mesh>& p_mesh);

public:
	//Particle Activable Count
	void SetParticleActivableCount(const UINT& activable_count);
	const UINT GetParticleActivableCount();

	//Particle Max Count
	void SetParticleMaxCount(const UINT& max_count);
	const UINT GetParticleMaxCount();

	//Particle Spawn Range
	void SetParticleSpawnRange(const Vector3& spawn_range);
	const Vector3 GetParticleSpawnRange();

	//Particle Scale
	void SetParticleStartScale(const Vector3& start_scale);
	const Vector3 GetParticleStartScale();

	void SetParticleEndScale(const Vector3& end_scale);
	const Vector3 GetParticleEndScale();

	//Particle Color
	void SetParticleStartColor(const Vector4& start_color);
	const Vector4 GetParticleStartColor();

	void SetParticleEndColor(const Vector4& end_color);
	const Vector4 GetParticleEndColor();

	//Particle Speed
	void SetParticleMinSpeed(const float& min_speed);
	const float GetParticleMinSpeed();

	void SetParticleMaxSpeed(const float& max_speed);
	const float GetParticleMaxSpeed();

	//Particle Life
	void SetParticleMinLife(const float& min_life);
	const float GetParticleMinLife();

	void SetParticleMaxLife(const float& max_life);
	const float GetParticleMaxLife();

	//Particle Spawn Frequency
	void SetParticleSpawnFrequency(const float& spawn_frequency);
	const float GetParticleSpawnFrequency();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(ParticleRenderer);

private:
	//Particle Shared
	std::shared_ptr<StructuredBuffer> m_p_particle_shared_buffer = nullptr;

	//Particle
	std::shared_ptr<Particle> m_p_current_particle = nullptr;

	std::shared_ptr<Material> m_p_material = nullptr;
	std::shared_ptr<Mesh> m_p_mesh = nullptr;

	float m_accumulate_time = 0.0f;
};

