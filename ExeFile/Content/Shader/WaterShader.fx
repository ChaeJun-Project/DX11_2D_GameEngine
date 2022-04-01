#ifndef WaterShader
#define WaterShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"
#include "ShaderFunc.fx"

#define WATER_HEIGHT g_float_0
#define REFRACT_SCALE g_float_1

#define NORMAL_TEXTURE g_texture_1

//Vertex Shader
VertexColorTextureOutputType VS_Water(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output = (VertexColorTextureOutputType) 0.0f;
   
    //Position
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV
    vs_output.uv = vs_input.uv;
 
    return vs_output;
}

//Pixel Shader
float4 PS_Water(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
   
    //�׷��� �ȼ��� ����
    float uv_offset_y = (float) WATER_HEIGHT / g_client_resolution.y;
  
    //ȭ�鿡�� �ش� ȿ���� �׷��� UV ��ǥ ���ϱ� => �ȼ� ��ǥ/�ػ�
    float2 screen_uv = ps_input.position.xy / g_client_resolution;
  
    //v��ǥ ����
    screen_uv.y = 1.0f - screen_uv.y;
    
    //�� �κ��� �� ����غ��� ��..
    screen_uv.y -= 2 * uv_offset_y;
   
    //������ ���
    //Normal Map Texture�� �̿�
    float4 normal_map = NORMAL_TEXTURE.Sample(g_sampler1, ps_input.uv);
    //normal_map�� ���� (0 ~ 1)������ ���ε� �̸� �䵿ġ�� ������ ǥ���ϱ� ���� (-1, 1)�� ��ȯ
    float3 normal = (normal_map * 2.0f) - 1.0f;
    
    screen_uv = screen_uv + (normal.xy * REFRACT_SCALE);
    
    //���� ����ġ�� ������ ����
    //x�� �������� �翷���� �Դٰ��� �ϴ� ������
    float x = cos(ps_input.uv.y * 2 * PI + (g_accumulate_time * 4 * PI)) * 0.001f;
    screen_uv.x += x;
   
    //���� UV ��ǥ�� Texture ���ø�
    ps_output_color = g_texture_0.Sample(g_sampler1, screen_uv);
    
    //���ø��� �ȼ��� ������ �Ķ����� ������ ������ ���� �������� ����
    ps_output_color = lerp(ps_output_color, float4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f);
   
    return ps_output_color;
}
#endif