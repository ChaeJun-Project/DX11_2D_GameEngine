#ifndef SnowUpdate
#define SnowUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

#define SNOW_MAX_COUNT g_int_0

#define SNOW_PARTICLE_WORLD_POSITION g_vector4_0

#define SNOW_PARTICLE_SPAWN_RANGE    g_vector4_1

#define SNOW_PARTICLE_START_SCALE    g_vector4_2
#define SNOW_PARTICLE_END_SCALE      g_vector4_3

#define SNOW_PARTICLE_START_COLOR    g_vector4_4
#define SNOW_PARTICLE_END_COLOR      g_vector4_5

#define SNOW_PARTICLE_MIN_SPEED      g_float_0
#define SNOW_PARTICLE_MAX_SPEED      g_float_1

#define SNOW_PARTICLE_MIN_LIFE       g_float_2
#define SNOW_PARTICLE_MAX_LIFE       g_float_3

RWStructuredBuffer<ParticleShared> g_particle_shared : register(u0);
RWStructuredBuffer<ParticleInfo> g_snow_particle : register(u1);

//Compute Shader는 여러 개의 스레드가 동시에 해당 함수를 처리하는 구조(병렬처리 구조)
//따라서 동기화 문제가 발생할 수 있기에 InterlockedCompareExchange 함수를 사용
[numthreads(100, 1, 1)]
void CS(int3 thread_id : SV_DispatchThreadID)
{
    //병렬처리할 스레드 숫자가 파티클 최대 개수보다 크거나 같은 경우
    if (SNOW_MAX_COUNT <= thread_id.x)
        return;
  
    //비활성화 파티클 생성시키기
    if (g_snow_particle[thread_id.x].is_active == 0 && g_particle_shared[0].activable_count != 0)
    {
        int origin = g_particle_shared[0].activable_count;
        int exchanged = 0;
        
        while (0 < origin)
        {
            int value = origin - 1;
            //g_particle_shared[0].activable_count가 origin과 값이 같은 경우
            //g_particle_shared[0].activable_count에 value 값을 대입하고
            //원래 g_particle_shared[0].activable_count의 값을 exchanged에 저장
            InterlockedCompareExchange(g_particle_shared[0].activable_count, origin, value, exchanged);
            
            //파티클 활성화
            if (exchanged == origin)
            {
                g_snow_particle[thread_id.x].is_active = 1;
                break;
            }
            
            origin = value;
        }
        
        
            //이번에 생성된 파티클의 경우
        if (g_snow_particle[thread_id.x].is_active == 1)
        {
            //랜덤            
            //thread_id를 활용해서, 스레드간에 겹치지 않는 픽셀 참조 위치를 만듦
            float key = (float) thread_id.x / 100.f;
            
            float3 random = float3(Rand(key + 0.2f), Rand(key + 0.4f), Rand(key + 0.6f)); // 0.0f ~ 1.0f 사이의 랜덤값 반환
        
            float3 object_world_position = SNOW_PARTICLE_WORLD_POSITION.xyz + g_view_position;
            float3 snow_spawn_range = SNOW_PARTICLE_SPAWN_RANGE.xyz;
              
            float3 offset_position = random;
            offset_position *= snow_spawn_range;
            
            offset_position = (offset_position - snow_spawn_range * 0.5f);
            object_world_position += offset_position;
            object_world_position.y += (g_game_resolution.y * 0.5f);
            
            g_snow_particle[thread_id.x].world_position = object_world_position;
            g_snow_particle[thread_id.x].view_scale = SNOW_PARTICLE_START_SCALE.xyz + (SNOW_PARTICLE_END_SCALE.xyz - SNOW_PARTICLE_START_SCALE.xyz) * random.z;
            g_snow_particle[thread_id.x].color = SNOW_PARTICLE_START_COLOR;
            g_snow_particle[thread_id.x].speed = SNOW_PARTICLE_MIN_SPEED + (SNOW_PARTICLE_MAX_SPEED - SNOW_PARTICLE_MIN_SPEED) * random.z;
            g_snow_particle[thread_id.x].direction = normalize(float3(0.0f, -1.0f, 0.0f));
        }
    }
    
    // 활성화 되어있는 파티클 업데이트
    else
    {
        g_snow_particle[thread_id.x].world_position += g_delta_time * g_snow_particle[thread_id.x].speed * g_snow_particle[thread_id.x].direction;
        if (g_snow_particle[thread_id.x].world_position.y <= (-(g_game_resolution.y * 0.5f) + 112.0f))
        {
            g_snow_particle[thread_id.x].is_active = 0;
        }
    }
}

#endif