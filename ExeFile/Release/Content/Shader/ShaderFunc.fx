#ifndef ShaderFunc
#define ShaderFunc

#include "LightStruct.fx"
#include "ConstantBuffer.fx"

void GetLightColor(int light_index, float3 world_position, inout LightColor light_color)
{
    //������ ���� ���� �������� �Ÿ� 
    float dis = 0.0f;
    //���� �޴� ������ ����
    float light_ratio = 0.0f;
    
    switch (g_light2D_array[light_index].light_type)
    {
        case 0: //Directional
            break;
        
        case 1: //Point
        {
                //������ ���� ���� �������� �Ÿ�   
                dis = distance(g_light2D_array[light_index].light_position, world_position);
            
                //���� ����
                //���� ���⸦ 1�̶� ���� �� ���� ����� ���� ������Ʈ���� �Ÿ��� �ݺ�� 
                //light_ratio = 1 - saturate(dis / g_light2D_array[light_index].light_range);
                
                //���� �޴� ������ ����
                //�ﰢ�Լ�(cos) ����(������Ʈ�� �ڻ��ι�Ģ, ������ ����� 0 ~ 1 ������ ��)
                //������ ���� ���� �������� �Ÿ��� ������ ������ ���� �� PI(3.14...)/2 ���� ����
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

//Noise Texture�� �ȼ� ������ 0 ~ 1������ ���� ���� ���ϴ� �Լ�
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

//key: 0 ~ 1 ���� ����
float Rand(in float key)
{
    // ���� ���� ��ġ, ����    
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