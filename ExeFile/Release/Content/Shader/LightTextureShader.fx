#ifndef LightTextureShader
#define LightTextureShader

#include "VertexStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

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
    vs_output.world_position = vs_output.position;
    
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
    float4 ps_output_color;
    
    LightColor light_color = (LightColor)0.0f; //
    
    for (uint i = 0; i < g_light2D_count; ++i)
        GetLightColor(i, ps_input.world_position, light_color);
    
    ps_output_color = g_Texture_0.Sample(Sampler1, ps_input.uv);
    
    //각 색상 성분에 맞는 rgb값을 곱함
    ps_output_color.rgb *= light_color.color.rgb;
 
    return ps_output_color;
}

#endif