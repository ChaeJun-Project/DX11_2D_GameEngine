#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

cbuffer Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vector2_0;
    float2 g_vector2_1;
    float2 g_vector2_2;
    float2 g_vector2_3;
    
    float4 g_vector4_0;
    float4 g_vector4_1;
    float4 g_vector4_2;
    float4 g_vector4_3;
    
    row_major matrix g_matrix_0;
    row_major matrix g_matrix_1;
    row_major matrix g_matrix_2;
    row_major matrix g_matrix_3;
}

//Texture2D: 텍스처 자원
Texture2D g_Texture_0 : register(t0);
Texture2D g_Texture_1 : register(t1);
Texture2D g_Texture_2 : register(t2);
Texture2D g_Texture_3 : register(t3);


//SamplerState: 도형에 셰이딩 작업이 이루어질 때 
//어떻게 텍스처의 픽셀이 사용되는 지를 수정할 수 있게 해줌
//ex) 물체가 너무 멀리 있어 8픽셀만큼의 영역을 차지하는 경우 
//이 객체를 사용하여 원래 텍스처의 어떤 픽셀 혹은 어떤 픽셀 조합을 사용해야 할지 결정
SamplerState Sampler1 : register(s0);
SamplerState Sampler2 : register(s1);


//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
    
    //World -> View -> Projection 순서대로 곱해줘야 정상적인 위치 값이 나옴
    //mul(x, y)
    //x가 vector 값이면 행 벡터(Row-Vector)로 처리
    //y가 vector 값이면 열 벡터(Column-Vector)로 처리
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color값 복사
    vs_output.color = vs_input.color;
    
    //UV값 복사
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}


//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color;
    
    ps_output_color = g_Texture_0.Sample(Sampler1, ps_input.uv);
 
    return ps_output_color;
}
