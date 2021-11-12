#ifndef ParticleUpdate
#define ParticleUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"

RWStructuredBuffer<ParticleInfo> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);

//Compute Shader�� ���� ���� �����尡 ���ÿ� �ش� �Լ��� ó���ϴ� ����(����ó�� ����)
//���� ����ȭ ������ �߻��� �� �ֱ⿡ InterlockedCompareExchange �Լ��� ���
[numthreads(100, 1, 1)]
void CS(int3 thread_id : SV_DispatchThreadID)
{
    //��Ȱ��ȭ ��ƼŬ ������Ű��
    //if (g_particle[thread_id.x].is_active == 0 && g_particle_shared[0].activable_count != 0)
    //{
    //    int origin = g_particle_shared[0].activable_count;
    //    int exchanged = 0;
        
    //    while (0 < origin)
    //    {
    //        int value = origin - 1;
    //        //InterlockedExchange(g_shared[0].iActivableCount, iValue, iExchanged);            
    //        InterlockedCompareExchange(g_particle_shared[0].activable_count, origin, value, exchanged);
            
    //        // ����
    //        if (exchanged == origin)
    //        {
    //            g_particle[thread_id.x].is_active = 1;
    //            break;
    //        }
            
    //        origin = value;
    //    }
        
        
    //    // �̹��� ������ ��ƼŬ�� ���
    //    if (g_particle[thread_id.x].is_active == 1)
    //    {
    //        // ����            
    //        // _id �� Ȱ���ؼ�, �����尣�� ��ġ�� �ʴ� �ȼ� ���� ��ġ�� ����� ����.
            
    //        // Sample_CS(, g_noise, g_vResolution_noise);
            
    //    }
    //}
    
    //// Ȱ��ȭ �Ǿ��ִ� ��ƼŬ ������Ʈ
    //else
    //{
        
    //} 
}

#endif