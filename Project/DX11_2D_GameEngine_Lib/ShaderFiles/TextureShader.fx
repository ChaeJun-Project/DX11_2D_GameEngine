#ifndef TextureShader
#define TextureShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

#define HAS_ANIMATION2D g_int_0
#define SPRITE_TEXTURE_COLOR g_vector4_0

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
    //���� �׷��� �޽��� ����� ��ġ
    vs_output.world_position = vs_output.position.xyz;
    
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color�� ����
    vs_output.color = vs_input.color;
    
    //UV�� ����
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

#include "ShaderFunc.fx"
//Pixel Shader
float4 PS(VertexColorTextureLightOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
  
    //SpriteAnimation
    if (HAS_ANIMATION2D)
    {
        SpriteAnimation_Data current_animation2D_data = g_sprite_animation;
        
        float2 animation2D_uv = (ps_input.uv * current_animation2D_data.frame_size);
          
        animation2D_uv += current_animation2D_data.left_top;
        ps_output_color = g_texture_0.Sample(g_sampler1, animation2D_uv);
    }
    else
    {
        ps_output_color = g_texture_0.Sample(g_sampler1, ps_input.uv);
    }
    
    ps_output_color.rgb *= SPRITE_TEXTURE_COLOR.rgb;
    
    //Light
    float4 light_color = (float4) 0.0f;
    
    for (uint i = 0; i < g_light2D_count; ++i)
        GetLight2DColor(i, ps_input.world_position, light_color);

    ps_output_color.rgb *= light_color.rgb;
  
    return ps_output_color;
}

#endif