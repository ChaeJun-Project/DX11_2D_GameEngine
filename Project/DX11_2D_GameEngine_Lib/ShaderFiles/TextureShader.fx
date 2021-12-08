#ifndef TextureShader
#define TextureShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

//RS State���� Solid ���
//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
    
    //World -> View -> Projection ������� ������� �������� ��ġ ���� ����
    //mul(x, y)
    //x�� vector ���̸� �� ����(Row-Vector)�� ó��
    //y�� vector ���̸� �� ����(Column-Vector)�� ó��
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color�� ����
    vs_output.color = vs_input.color;
    
    //UV�� ����
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    ps_output_color = g_texture_0.Sample(g_sampler1, ps_input.uv);
   
    return ps_output_color;
}

#endif