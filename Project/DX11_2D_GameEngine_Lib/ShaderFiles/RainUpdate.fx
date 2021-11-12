#ifndef RainUpdate
#define RainUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

RWStructuredBuffer<ParticleInfo> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);

//Compute Shader�� ���� ���� �����尡 ���ÿ� �ش� �Լ��� ó���ϴ� ����(����ó�� ����)
//���� ����ȭ ������ �߻��� �� �ֱ⿡ InterlockedCompareExchange �Լ��� ���
[numthreads(100, 1, 1)]
void CS(int3 thread_id : SV_DispatchThreadID)
{
    //����ó���� ������ ���ڰ� ��ƼŬ �ִ� �������� ũ�ų� ���� ���
    if (max_count <= thread_id.x)
        return;
    
    //��Ȱ��ȭ ��ƼŬ ������Ű��
    if (g_particle[thread_id.x].is_active == 0 && g_particle_shared[0].activable_count != 0)
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
                g_particle[thread_id.x].is_active = 1;
                break;
            }
            
            origin = value;
        }
        
        
        // �̹��� ������ ��ƼŬ�� ���
        if (g_particle[thread_id.x].is_active == 1)
        {
            //����            
            //thread_id�� Ȱ���ؼ�, �����尣�� ��ġ�� �ʴ� �ȼ� ���� ��ġ�� ����
            float key = (float) thread_id.x / 100.f;
            
            float3 random = float3(Rand(key), Rand(key + 0.1f), Rand(key + 0.2f)); // 0.0f ~ 1.0f ������ ������ ��ȯ
            
            float3 offset_position = random;
            offset_position *= spawn_range;
            
            float3 object_world_position = world_position;
            offset_position = (offset_position - (spawn_range * 0.5f));
            object_world_position += offset_position;
            
            g_particle[thread_id.x].world_position = object_world_position;
            g_particle[thread_id.x].view_scale = start_scale;
            g_particle[thread_id.x].direction = normalize(float3(0.0f, -1.0f, 0.0f));
            g_particle[thread_id.x].life_time = max_life;
        }
    }
    
    // Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ������Ʈ
    else
    {
        g_particle[thread_id.x].current_time += g_delta_time;

        if (g_particle[thread_id.x].life_time <= g_particle[thread_id.x].current_time)
        {
            g_particle[thread_id.x].is_active = 0;
            g_particle[thread_id.x].current_time = 0.0f;
        }
        
        g_particle[thread_id.x].world_position += g_delta_time * speed * g_particle[thread_id.x].direction;
    }
}

#endif