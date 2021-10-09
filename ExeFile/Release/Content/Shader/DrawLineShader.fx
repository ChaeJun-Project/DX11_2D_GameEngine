#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexColorOuputType VS(VertexColor vs_input)
{
    VertexColorOuputType vs_output;
    
     //World -> View -> Projection ������� ������� �������� ��ġ ���� ����
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV�� ����
    vs_output.color = vs_input.color;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorOuputType ps_input) : SV_Target
{
    return ps_input.color;
}