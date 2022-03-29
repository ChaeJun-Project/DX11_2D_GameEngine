#ifndef Light2DShader
#define Light2DShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
   
    //Position
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

#include "ShaderFunc.fx"
//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    //TODO
    // //Light
    //float4 light_color = (float4) 0.0f;
    
    //for (uint i = 0; i < g_light2D_count; ++i)
    //    GetLight2DColor(i, ps_input.world_position, light_color);
    
    ////각 색상 성분에 맞는 rgb값을 곱함
    //ps_output_color.rgb *= light_color.rgb;
 
    return ps_output_color;
}

#endif