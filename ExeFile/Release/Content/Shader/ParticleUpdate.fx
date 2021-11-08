#ifndef ParticleUpdate
#define ParticleUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"

RWStructuredBuffer<ParticleInfo> g_particle : register(u0);

[numthreads(32, 1, 1)]
void CS(int thread_id : SV_DispatchThreadID)
{
    
}

#endif