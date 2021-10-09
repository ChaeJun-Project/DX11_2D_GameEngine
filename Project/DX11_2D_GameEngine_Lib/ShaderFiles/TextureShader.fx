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

//Texture2D: �ؽ�ó �ڿ�
Texture2D g_Texture_0 : register(t0);
Texture2D g_Texture_1 : register(t1);
Texture2D g_Texture_2 : register(t2);
Texture2D g_Texture_3 : register(t3);


//SamplerState: ������ ���̵� �۾��� �̷���� �� 
//��� �ؽ�ó�� �ȼ��� ���Ǵ� ���� ������ �� �ְ� ����
//ex) ��ü�� �ʹ� �ָ� �־� 8�ȼ���ŭ�� ������ �����ϴ� ��� 
//�� ��ü�� ����Ͽ� ���� �ؽ�ó�� � �ȼ� Ȥ�� � �ȼ� ������ ����ؾ� ���� ����
SamplerState Sampler1 : register(s0);
SamplerState Sampler2 : register(s1);


//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
    
    //World -> View -> Projection ������� ������� �������� ��ġ ���� ����
    //mul(x, y)
    //x�� vector ���̸� �� ����(Row-Vector)�� ó��
    //y�� vector ���̸� �� ����(Column-Vector)�� ó��
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color�� ����
    vs_output.color = vs_input.color;
    
    //UV�� ����
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}


//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    return g_Texture_0.Sample(Sampler1, ps_input.uv);
}
