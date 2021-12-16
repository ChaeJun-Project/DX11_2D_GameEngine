#ifndef DrawLineShader
#define DrawLineShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

//RS State에서 WireFrame 사용
//Vertex Shader
VertexColorOuputType VS(VertexColor vs_input)
{
    VertexColorOuputType vs_output;
    
    //World -> View -> Projection 순서대로 곱해줘야 정상적인 위치 값이 나옴
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color값 복사
    vs_output.color = vs_input.color;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorOuputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    ps_output_color = ps_input.color;
    ps_output_color.a = 1.0f;
  
    //Collider2D Green Line
    if (g_int_0)
    {
        ps_output_color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    }
   
    //Collider2D Red Line
    if (g_int_1)
    {
        ps_output_color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
   
    return ps_output_color;
}

#endif