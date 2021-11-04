#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	:IComponent(ComponentType::ParticleSystem)
{
}

ParticleSystem::ParticleSystem(const ParticleSystem& origin)
	: IComponent(origin.GetComponentType())
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::FinalUpdate()
{
}

void ParticleSystem::BindPipeline()
{
}

void ParticleSystem::Render()
{
}
