#ifndef ShaderFunc
#define ShaderFunc

#include "LightStruct.fx"
#include "ConstantBuffer.fx"

void GetLightColor(int light_index, float3 world_position, inout LightColor light_color)
{
    //광원과 빛을 받을 정점과의 거리 
    float dis = 0.0f;
    //빛을 받는 정도의 비율
    float light_ratio = 0.0f;
    
    switch (g_light2D_array[light_index].light_type)
    {
        case 0: //Directional
            break;
        
        case 1: //Point
        {
                //광원과 빛을 받을 정점과의 거리   
                dis = distance(g_light2D_array[light_index].light_position, world_position);
            
                //선형 보간
                //빛의 세기를 1이라 했을 때 빛의 세기는 빛과 오브젝트와의 거리에 반비례 
                //light_ratio = 1 - saturate(dis / g_light2D_array[light_index].light_range);
                
                //빛을 받는 정도의 비율
                //삼각함수(cos) 보간(람베르트의 코사인법칙, 조명의 세기는 0 ~ 1 사이의 값)
                //광원과 빛을 받을 정점과의 거리를 광원의 범위로 나눈 후 PI(3.14...)/2 값을 곱함
                light_ratio = cos(saturate(dis / g_light2D_array[light_index].light_range) * (PI / 2));
            }
            break;
        
        case 2: //Spot
            break;
    }
    
    light_color.color += g_light2D_array[light_index].ligth_color.color * light_ratio;
}

#endif