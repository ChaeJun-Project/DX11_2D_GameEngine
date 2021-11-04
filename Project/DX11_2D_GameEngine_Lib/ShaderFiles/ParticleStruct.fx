#ifndef ParticleStruct
#define ParticleStruct

struct ParticleInfo
{
    float3 particle_position;
    float4 particle_color;
    float3 particle_scale;
    
    float particle_current_time;
    float particle_life_time;  
};

#endif