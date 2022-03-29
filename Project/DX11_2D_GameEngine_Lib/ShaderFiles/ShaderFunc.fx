#ifndef ShaderFunc
#define ShaderFunc

#include "Light2DStruct.fx"
#include "ConstantBuffer.fx"

void GetLight2DColor(int light2D_index, float3 world_position, inout float4 light2D_color)
{
    switch (g_light2D_array[light2D_index].type)
    {
        case 0: //Ambient
        {
                light2D_color = g_light2D_array[light2D_index].color;
            }
            break;
        
        case 1: //Point
        {
                //광원과 빛을 받을 정점과의 거리 
                float dis = 0.0f;
                //빛을 받는 정도의 비율
                float light_ratio = 0.0f;
    
                //광원과 빛을 받을 정점과의 거리   
                dis = distance(g_light2D_array[light2D_index].position, world_position);
           
                //빛을 받는 정도의 비율
                //삼각함수(cos) 보간(람베르트의 코사인법칙, 조명의 세기는 0 ~ 1 사이의 값)
                //광원과 빛을 받을 정점과의 거리를 광원의 범위로 나눈 후 (PI(3.14...)/2 = 90도) 값을 곱함
                light_ratio = cos(saturate(dis / g_light2D_array[light2D_index].range) * (PI / 2));
            
                light2D_color = g_light2D_array[light2D_index].color * light_ratio;
            }
            break;
    }
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
float4 Sample_CS(in int2 center_pixel_position, in Texture2D noise_texture, int2 noise_texture_resolution)
{
    float4 value_out = (float4) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 iPixelPos = center_pixel_position + int2(i - 2, j - 2);
            if (iPixelPos.x < 0 || noise_texture_resolution.x <= iPixelPos.x
                || iPixelPos.y < 0 || noise_texture_resolution.y <= iPixelPos.y)
            {
                continue;
            }
            
            value_out += noise_texture[iPixelPos] * gaussian5x5[i * 5 + j];
        }
    }
    
    return value_out;
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