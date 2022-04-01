#ifndef SnowShader
#define SnowShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"
#include "ParticleStruct.fx"

struct VS_IN
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID; //인스턴스 인덱스
};

struct VS_OUT // = GS_IN
{
    float3 position : POSITION;
    uint instanceID : SV_InstanceID; //인스턴스 인덱스
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
    //파티클이 활성화 상태가 아니라면 GS 종료
    //GS에서 반환되는 정점의 정보가 없으므로 RS -> PS 단계가 진행이 되지 않아 해당 정점은 화면에 그려지지 않음
    if (g_snow_particle[gs_input[0].instanceID].is_active == 0)
    {
        return;
    }
   
    //출력시킬 4개의 정점
    VertexColorTextureOutputType gs_output[4];
    
    //월드 좌표계에서의 파티클 위치
    float3 particle_world_position = g_snow_particle[gs_input[0].instanceID].world_position;
   
    float3 particle_scale = g_snow_particle[gs_input[0].instanceID].view_scale;
    
    //Rotation은 추후 처리
    float3 particle_rotation = g_snow_particle[gs_input[0].instanceID].view_rotation;
  
    //View 좌표계에서의 파티클 위치
    float4 particle_view_position = mul(float4(particle_world_position, 1.0f), view);
    
     //점을 중심으로 4개의 정점 생성
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
    
    //UV좌표에 대응하는 텍스처의 알파값 추출
    float texture_alpha = g_texture_0.Sample(g_sampler1, ps_input.uv).a;
    
    //색상 정보 전달
    ps_output_color = ps_input.color;
    ps_output_color.a = texture_alpha;
 
    return ps_output_color;
}

#endif