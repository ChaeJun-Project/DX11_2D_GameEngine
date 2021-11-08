#pragma once
#include "IComponent.h"

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

	void Initialize();

	virtual void FinalUpdate() override;

	void BindPipeline();

	void Render();

public:
    void SetMaxParticleCount(const UINT& max_particle_count) { m_max_particle_count = max_particle_count; }

public:
    CLONE(ParticleSystem);

private:
	std::shared_ptr<Mesh> m_p_mesh = nullptr;
	std::shared_ptr<Material> m_p_material = nullptr;

	std::shared_ptr<ParticleUpdateShader> m_p_particle_update_shader = nullptr;
	std::shared_ptr<StructuredBuffer> m_p_structured_buffer = nullptr;

	UINT m_max_particle_count = 10;
};

