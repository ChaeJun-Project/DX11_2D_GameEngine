#ifndef ParticleUpdate
#define ParticleUpdate

#include "ParticleStruct.fx"
#include "ConstantBuffer.fx"

RWStructuredBuffer<ParticleInfo> g_particle : register(u0);
RWStructuredBuffer<ParticleShared> g_particle_shared : register(u1);

//Compute Shader는 여러 개의 스레드가 동시에 해당 함수를 처리하는 구조(병렬처리 구조)
//따라서 동기화 문제가 발생할 수 있기에 InterlockedCompareExchange 함수를 사용
[numthreads(100, 1, 1)]
void CS(int3 thread_id : SV_DispatchThreadID)
{
    //비활성화 파티클 생성시키기
    //if (g_particle[thread_id.x].is_active == 0 && g_particle_shared[0].activable_count != 0)
    //{
    //    int origin = g_particle_shared[0].activable_count;
    //    int exchanged = 0;
        
    //    while (0 < origin)
    //    {
    //        int value = origin - 1;
    //        //InterlockedExchange(g_shared[0].iActivableCount, iValue, iExchanged);            
    //        InterlockedCompareExchange(g_particle_shared[0].activable_count, origin, value, exchanged);
            
    //        // 성공
    //        if (exchanged == origin)
    //        {
    //            g_particle[thread_id.x].is_active = 1;
    //            break;
    //        }
            
    //        origin = value;
    //    }
        
        
    //    // 이번에 생성된 파티클의 경우
    //    if (g_particle[thread_id.x].is_active == 1)
    //    {
    //        // 랜덤            
    //        // _id 를 활용해서, 스레드간에 겹치지 않는 픽셀 참조 위치를 만들어 낸다.
            
    //        // Sample_CS(, g_noise, g_vResolution_noise);
            
    //    }
    //}
    
    //// 활성화 되어있는 파티클 업데이트
    //else
    //{
        
    //} 
}

#endif