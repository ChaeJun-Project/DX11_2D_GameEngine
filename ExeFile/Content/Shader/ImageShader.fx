#ifndef ImageShader
#define ImageShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

#include "ImageFunc.fx"

#define HAS_TEXTURE g_int_0
#define IMAGE_TYPE g_int_1 //1: Filled

#define IMAGE_COLOR g_vector4_0

#define FILL_METHOD g_int_2 //1: Horizontal(수평) 2:Vertical(수직)
#define FILL_DIRECTION g_int_3 //FILL_METHOD == Horizontal(수평)(1:Left 2:Right) FILL_METHOD == Vertical(수직)(1:Bottom 2:Top)
#define FILL_AMOUNT g_float_0

//RS State에서 Solid 사용
//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
    
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color값 복사
    vs_output.color = vs_input.color;

    //UV값 복사
    vs_output.uv = vs_input.uv;
    
    return vs_output; 
}

float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;

    ps_output_color = ps_input.color;
    
    //UI에 사용될 Texture가 존재하는 경우
    if (HAS_TEXTURE)
    {
        ps_output_color = g_texture_0.Sample(g_sampler1, ps_input.uv);

        //Filled
        if(IMAGE_TYPE == 1)
        {
            if (!FillImage(FILL_METHOD, FILL_DIRECTION, FILL_AMOUNT, ps_input.uv))
                clip(-1);
        } 
    }
    
    ps_output_color *= IMAGE_COLOR;
 
    return ps_output_color;
}

#endif