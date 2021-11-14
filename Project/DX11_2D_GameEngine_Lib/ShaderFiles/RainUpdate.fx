#ifndef RainUpdate
#define RainUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

#define ADD_SPAWN_RANGE 1000;

#define MAX_COUNT g_int_0

#define RAIN_PARTICLE_WORLD_POSITION g_vector4_0

#define RAIN_PARTICLE_SPAWN_RANGE    g_vector4_1

#define RAIN_PARTICLE_START_SCALE    g_vector4_2
#define RAIN_PARTICLE_END_SCALE      g_vector4_3

#define RAIN_PARTICLE_START_COLOR    g_vector4_4

#define RAIN_PARTICLE_MIN_SPEED          g_float_0
#define RAIN_PARTICLE_MAX_SPEED          g_float_1

RWStructuredBuffer<ParticleInfo> g_rain_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);

//Compute Shader�� ���� ���� �����尡 ���ÿ� �ش� �Լ��� ó���ϴ� ����(����ó�� ����)
//���� ����ȭ ������ �߻��� �� �ֱ⿡ InterlockedCompareExchange �Լ��� ���
[numthreads(100, 1, 1)]
void CS_Rain(int3 thread_id : SV_DispatchThreadID)
{
    //����ó���� ������ ���ڰ� ��ƼŬ �ִ� �������� ũ�ų� ���� ���
    if (MAX_COUNT <= thread_id.x)
        return;
    
    //��Ȱ��ȭ ��ƼŬ ������Ű��
    if (g_rain_particle[thread_id.x].is_active == 0 && g_particle_shared[0].activable_count != 0)
    {
        int origin = g_particle_shared[0].activable_count;
        int exchanged = 0;
        
        while (0 < origin)
        {
            int value = origin - 1;
            //InterlockedExchange(g_shared[0].iActivableCount, iValue, iExchanged);            
            InterlockedCompareExchange(g_particle_shared[0].activable_count, origin, value, exchanged);
            
            // ����
            if (exchanged == origin)
            {
                g_rain_particle[thread_id.x].is_active = 1;
                break;
            }
            
            origin = value;
        }
        
        
        // �̹��� ������ ��ƼŬ�� ���
        if (g_rain_particle[thread_id.x].is_active == 1)
        {
            //����            
            //thread_id�� Ȱ���ؼ�, �����尣�� ��ġ�� �ʴ� �ȼ� ���� ��ġ�� ����
            float key = (float) thread_id.x / 100.f;
            
            float3 random = float3(Rand(key), Rand(key + 0.2f), Rand(key + 0.4f)); // 0.0f ~ 1.0f ������ ������ ��ȯ
        
            float3 rain_spawn_range = RAIN_PARTICLE_SPAWN_RANGE.xyz;
            rain_spawn_range.x += (float) ADD_SPAWN_RANGE;
            
            float3 offset_position = random;
            offset_position *= rain_spawn_range;
            
            float3 object_world_position = RAIN_PARTICLE_WORLD_POSITION.xyz + g_view_position;
            object_world_position.y += (g_resolution.y);
          
            offset_position = (offset_position - rain_spawn_range * 0.5f);
            object_world_position += offset_position;
            
            g_rain_particle[thread_id.x].world_position = object_world_position;
            g_rain_particle[thread_id.x].view_scale = RAIN_PARTICLE_START_SCALE.xyz;
            g_rain_particle[thread_id.x].color = RAIN_PARTICLE_START_COLOR;
            g_rain_particle[thread_id.x].speed = RAIN_PARTICLE_MIN_SPEED + (RAIN_PARTICLE_MAX_SPEED - RAIN_PARTICLE_MIN_SPEED) * random.x;
            g_rain_particle[thread_id.x].direction = normalize(float3(0.0f, -1.0f, 0.0f));
        }
    }
    
    // Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ������Ʈ
    else
    {
        g_rain_particle[thread_id.x].world_position += g_delta_time * g_rain_particle[thread_id.x].speed * g_rain_particle[thread_id.x].direction;
        if (g_rain_particle[thread_id.x].world_position.y <= (-(g_resolution.y * 0.5f)))
        {
            g_rain_particle[thread_id.x].is_active = 0;
        }
    }
}

#endif