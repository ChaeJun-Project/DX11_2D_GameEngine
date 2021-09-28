#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexTextureOutputType VS(VertexTexture vs_input)
{
    VertexTextureOutputType vs_output;
    
    //World -> View -> Projection ������� ������� �������� ��ġ ���� ����
    vs_output.position = mul(vs_input.position, world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV�� ����
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//SamplerState: ������ ���̵� �۾��� �̷���� �� 
//��� �ؽ�ó�� �ȼ��� ���Ǵ� ���� ������ �� �ְ� ����
//ex) ��ü�� �ʹ� �ָ� �־� 8�ȼ���ŭ�� ������ �����ϴ� ��� 
//�� ��ü�� ����Ͽ� ���� �ؽ�ó�� � �ȼ� Ȥ�� � �ȼ� ������ ����ؾ� ���� ����
SamplerState Sampler : register(s0);
//Texture2D: �ؽ�ó �ڿ�
Texture2D Texture : register(t0); 

//Pixel Shader
float4 PS(VertexTextureOutputType ps_input) : SV_Target
{
    return Texture.Sample(Sampler, ps_input.uv);
}
