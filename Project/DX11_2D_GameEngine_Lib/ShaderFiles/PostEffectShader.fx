#ifndef PostEffectShader
#define PostEffectShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output = (VertexColorTextureOutputType) 0.0f;
   
    //Position
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
 
    float x = sin(ps_input.uv.y * 2 * PI + (g_accumulate_time * 4 * PI)) * 0.01f;
    
    // �ȼ� ��ǥ /  �ػ� ==> ����Ÿ�� ��ü ȭ���� �������� �� UV ��
    float2 screen_uv = ps_input.position.xy / g_resolution;
    screen_uv.x += x;
       
    ps_output_color = g_texture_0.Sample(g_sampler1, screen_uv);
 
    return ps_output_color;
}
#endif