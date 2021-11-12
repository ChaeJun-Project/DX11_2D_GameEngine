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


static float gaussian5x5[25] =
{
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

//Noise Texture의 픽셀 값으로 0 ~ 1사이의 랜덤 값을 구하는 함수
float4 Sample_CS(in int2 _CenterPixelPos, in Texture2D _tex, int2 _resolution)
{
    float4 fOut = (float4) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 iPixelPos = _CenterPixelPos + int2(i - 2, j - 2);
            if (iPixelPos.x < 0 || _resolution.x <= iPixelPos.x
                || iPixelPos.y < 0 || _resolution.y <= iPixelPos.y)
            {
                continue;
            }
            
            fOut += _tex[iPixelPos] * gaussian5x5[i * 5 + j];
        }
    }
    
    return fOut;
}

//key: 0 ~ 1 사이 랜덤
float Rand(in float key)
{
    // 랜덤 생성 위치, 방향    
    float2 uv = float2(key + g_accumulate_time, g_accumulate_time);
    uv.y += sin(uv.x * 2 * 3.141592);
            
    if (uv.x > 0)
        uv.x = frac(uv.x);
    else
        uv.x = 1.f - abs(uv.x);
            
    if (uv.y > 0)
        uv.y = frac(uv.y);
    else
        uv.y = 1.f - abs(uv.y);
        
    int2 pixel_pos = (int2) (g_noise_resolution * uv);
    
    return Sample_CS(pixel_pos, g_noise_texture, g_noise_resolution).x;
}

#endif