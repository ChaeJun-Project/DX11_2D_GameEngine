#ifndef LightTextureShader
#define LightTextureShader

#include "VertexStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

//RS State���� Solid ���
//Vertex Shader
VertexColorTextureLightOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureLightOutputType vs_output;
    
    //World -> View -> Projection ������� ������� �������� ��ġ ���� ����
    //mul(x, y)
    //x�� vector ���̸� �� ����(Row-Vector)�� ó��
    //y�� vector ���̸� �� ����(Column-Vector)�� ó��
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.world_position = vs_output.position;
    
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color�� ����
    vs_output.color = vs_input.color;
    
    //UV�� ����
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureLightOutputType ps_input) : SV_Target
{
    float4 ps_output_color;
    
    LightColor light_color = (LightColor)0.0f; //
    
    for (uint i = 0; i < g_light2D_count; ++i)
        GetLightColor(i, ps_input.world_position, light_color);
    
    ps_output_color = g_Texture_0.Sample(Sampler1, ps_input.uv);
    
    //�� ���� ���п� �´� rgb���� ����
    ps_output_color.rgb *= light_color.color.rgb;
 
    return ps_output_color;
}

#endif