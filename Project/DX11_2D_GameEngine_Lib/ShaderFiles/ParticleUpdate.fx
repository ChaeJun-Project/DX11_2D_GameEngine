#ifndef ParticleUpdate
#define ParticleUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"

RWStructuredBuffer<ParticleInfo> g_particle : register(u0);

[numthread(32, 1, 1)]
void CS()
{
    
}

#endif