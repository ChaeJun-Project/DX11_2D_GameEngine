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

#endif