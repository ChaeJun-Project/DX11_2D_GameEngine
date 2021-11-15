#ifndef PostEffectShader
#define PostEffectShader

#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output = (VertexColorTextureOutputType) 0.0f;
   
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float)0.0f;
 
    return ps_output_color;
}
#endif