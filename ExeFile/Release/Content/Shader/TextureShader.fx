#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

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

//RS State���� Solid ���
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
    float4 ps_output_color;
    
    ps_output_color = g_Texture_0.Sample(Sampler1, ps_input.uv);
 
    return ps_output_color;
}
