#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorOuputType VS(VertexColor vs_input)
{
    VertexColorOuputType vs_output;
    
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorOuputType ps_input) : SV_Target
{
    return ps_input.color;
}