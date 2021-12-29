#ifndef LightTextureShader
#define LightTextureShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

#define HAS_ANIMATION2D g_int_0
#define ANIMTATOR2D_INDEX g_int_1

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

    //Animation2D
    if (HAS_ANIMATION2D)
    {
        Animation2D_Data current_animation2D_data = g_animation2D_data_array[ANIMTATOR2D_INDEX];
        
        float2 animation2D_uv = (ps_input.uv * current_animation2D_data.full_frame_size);
        float2 uv_range = (current_animation2D_data.full_frame_size - current_animation2D_data.frame_size) / 2.f + current_animation2D_data.offset;
                
        if (uv_range.x < animation2D_uv.x && animation2D_uv.x < uv_range.x + current_animation2D_data.frame_size.x
            && uv_range.y < animation2D_uv.y && animation2D_uv.y < uv_range.y + current_animation2D_data.frame_size.y)
        {
            animation2D_uv += (current_animation2D_data.left_top - uv_range + current_animation2D_data.frame_size / 2 - current_animation2D_data.offset);
            ps_output_color = g_texture_0.Sample(g_sampler1, animation2D_uv);
        }
        
        else
            clip(-1); // 호출 된 픽셀 쉐이더 폐기처분
    }
    else
    {
        ps_output_color = g_texture_0.Sample(g_sampler1, ps_input.uv);
    }
    
    ps_output_color = g_texture_0.Sample(g_sampler1, ps_input.uv);
    //각 색상 성분에 맞는 rgb값을 곱함
    ps_output_color.rgb *= light_color.color.rgb;
 
    return ps_output_color;
}

#endif