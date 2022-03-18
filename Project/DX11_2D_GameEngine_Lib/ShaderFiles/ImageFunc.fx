#ifndef ImageFunc
#define ImageFunc

#include "ConstantBuffer.fx"

bool FillHorizontal(int fill_direction, float fill_amount, float2 image_uv)
{
    if (fill_direction == 1) //���ʺ��� ä��
    {
        if (image_uv.x <= fill_amount)
            return true;
        
        return false;
    }
    
    else //�����ʺ��� ä��
    {
        float relative_fill_amount = 1.0f - fill_amount;
        
        if (image_uv.x >= relative_fill_amount)
            return true;
        
        return false;
    }
}

bool FillVertical(int fill_direction, float fill_amount, float2 image_uv)
{
    if (fill_direction == 1) //�ٴں��� ä��
    {
        float relative_fill_amount = 1.0f - fill_amount;
        
        if (image_uv.y >= relative_fill_amount)
            return true;
        
        return false;
    }
    else //������ ä��
    {
        if (image_uv.y <= fill_amount)
            return true;
        
        return false;
    }
}

bool FillImage(int fill_method, int fill_direction, float fill_amount, float2 image_uv)
{
    switch (fill_method)
    {
        case 1: //Horizontal(����)
        {
                return FillHorizontal(fill_direction, fill_amount, image_uv);

            }
            break;
        case 2: //2:Vertical(����)
        {
                FillVertical(fill_direction, fill_amount, image_uv);

            }
            break;
    }
    
    return false;
}

#endif