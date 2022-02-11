#ifndef LightTextureShader
#define LightTextureShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

#define HAS_ANIMATION2D g_int_0
#define SPRITE_TEXTURE_COLOR g_vector4_0

//RS State에서 Solid 사용
//Vertex Shader
VertexColorTextureLightOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureLightOutputType vs_output;
   
    //World -> View -> Projection 순서대로 곱해줘야 정상적인 위치 값이 나옴
    //mul(x, y)
    //x가 vector 값이면 행 벡터(Row-Vector)로 처리
    //y가 vector 값이면 열 벡터(Column-Vector)로 처리
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.world_position = vs_output.position.xyz;
    
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color값 복사
    vs_output.color = vs_input.color;
    
    //UV값 복사
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureLightOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    //Light
    LightColor light_color = (LightColor) 0.0f;
    
    for (uint i = 0; i < g_light2D_count; ++i)
        GetLightColor(i, ps_input.world_position, light_color);

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

    //각 색상 성분에 맞는 rgb값을 곱함
    ps_output_color.rgb *= light_color.color.rgb;
    
    ps_output_color *= SPRITE_TEXTURE_COLOR;
 
    return ps_output_color;
}

#endif