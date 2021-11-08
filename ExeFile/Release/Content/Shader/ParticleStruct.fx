#ifndef ParticleStruct
#define ParticleStruct

struct ParticleInfo
{
    float3 particle_world_position;
    float3 particle_rotation;
    float3 particle_scale;
    
    float4 particle_color;
    
    float particle_current_time;
    float particle_life_time;  
    int particle_active;
};

#endif