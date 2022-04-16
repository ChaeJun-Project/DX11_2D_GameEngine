#ifndef Light2DShader
#define Light2DShader

#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorTextureLightOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureLightOutputType vs_output;
  
    //Position
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
 
    //실제 그려질 메쉬의 월드상 위치
    vs_output.world_position = vs_output.position.xyz;
    vs_output.world_position.xy = vs_output.world_position.xy / g_client_resolution;
    vs_output.world_position.xy *= g_game_resolution;
    vs_output.world_position += g_view_position;
    
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

#include "ShaderFunc.fx"
//Pixel Shader
float4 PS(VertexColorTextureLightOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    float2 screen_uv = ps_input.position.xy / g_client_resolution;
    
    ps_output_color = g_texture_0.Sample(g_sampler1, screen_uv);
   
     //Light
    float4 light_color = (float4) 0.0f;
    
    for (uint i = 0; i < g_light2D_count; ++i)
        GetLight2DColor(i, ps_input.world_position, light_color);

    ps_output_color.rgb *= light_color.rgb;
  
    return ps_output_color;
}

#endif