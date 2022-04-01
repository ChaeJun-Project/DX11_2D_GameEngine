#ifndef SnowShader
#define SnowShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"
#include "ParticleStruct.fx"

struct VS_IN
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID; //�ν��Ͻ� �ε���
};

struct VS_OUT // = GS_IN
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID; //�ν��Ͻ� �ε���
};

//Vertex Shader
VS_OUT VS(VS_IN vs_input)
{
    VS_OUT vs_output = (VS_OUT) 0.0f;
  
    vs_output.position = vs_input.position;
    vs_output.instanceID = vs_input.instanceID;
  
    return vs_output;
}

StructuredBuffer<ParticleInfo> g_snow_particle : register(t14);

[maxvertexcount(6)]
//Geometry Shader
void GS(point VS_OUT gs_input[1], inout TriangleStream<VertexColorTextureOutputType> gs_output_stream)
{
    //��ƼŬ�� Ȱ��ȭ ���°� �ƴ϶�� GS ����
    //GS���� ��ȯ�Ǵ� ������ ������ �����Ƿ� RS -> PS �ܰ谡 ������ ���� �ʾ� �ش� ������ ȭ�鿡 �׷����� ����
    if (g_snow_particle[gs_input[0].instanceID].is_active == 0)
    {
        return;
    }
   
    //��½�ų 4���� ����
    VertexColorTextureOutputType gs_output[4];
    
    //���� ��ǥ�迡���� ��ƼŬ ��ġ
    float3 particle_world_position = g_snow_particle[gs_input[0].instanceID].world_position;
   
    float3 particle_scale = g_snow_particle[gs_input[0].instanceID].view_scale;
    
    //Rotation�� ���� ó��
    float3 particle_rotation = g_snow_particle[gs_input[0].instanceID].view_rotation;
  
    //View ��ǥ�迡���� ��ƼŬ ��ġ
    float4 particle_view_position = mul(float4(particle_world_position, 1.0f), view);
    
     //���� �߽����� 4���� ���� ����
    // 0 -- 1
    // | \  |
    // 3 -- 2
    //0
    gs_output[0].position = float4(particle_view_position.x - (particle_scale.x * 0.5f), particle_view_position.y + (particle_scale.y * 0.5f), particle_view_position.z, 1.0f);
    gs_output[0].position = mul(gs_output[0].position, projection);
    gs_output[0].color = g_snow_particle[gs_input[0].instanceID].color;
    gs_output[0].uv = float2(0.0f, 0.0f);
    
    //1
    gs_output[1].position = float4(particle_view_position.x + (particle_scale.x * 0.5f), particle_view_position.y + (particle_scale.y * 0.5f), particle_view_position.z, 1.0f);
    gs_output[1].position = mul(gs_output[1].position, projection);
    gs_output[1].color = g_snow_particle[gs_input[0].instanceID].color;
    gs_output[1].uv = float2(1.0f, 0.0f);
    
    //2
    gs_output[2].position = float4(particle_view_position.x + (particle_scale.x * 0.5f), particle_view_position.y - (particle_scale.y * 0.5f), particle_view_position.z, 1.0f);
    gs_output[2].position = mul(gs_output[2].position, projection);
    gs_output[2].color = g_snow_particle[gs_input[0].instanceID].color;
    gs_output[2].uv = float2(1.0f, 1.0f);
    
    //3
    gs_output[3].position = float4(particle_view_position.x - (particle_scale.x * 0.5f), particle_view_position.y - (particle_scale.y * 0.5f), particle_view_position.z, 1.0f);
    gs_output[3].position = mul(gs_output[3].position, projection);
    gs_output[3].color = g_snow_particle[gs_input[0].instanceID].color;
    gs_output[3].uv = float2(0.0f, 1.0f);

    //Triangle1
    gs_output_stream.Append(gs_output[0]);
    gs_output_stream.Append(gs_output[1]);
    gs_output_stream.Append(gs_output[2]);
    gs_output_stream.RestartStrip();
    
    //Triangle2
    gs_output_stream.Append(gs_output[0]);
    gs_output_stream.Append(gs_output[2]);
    gs_output_stream.Append(gs_output[3]);
    gs_output_stream.RestartStrip();
}

//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
    
    //UV��ǥ�� �����ϴ� �ؽ�ó�� ���İ� ����
    float texture_alpha = g_texture_0.Sample(g_sampler1, ps_input.uv).a;
    
    //���� ���� ����
    ps_output_color = ps_input.color;
    ps_output_color.a = texture_alpha;
 
    return ps_output_color;
}

#endif