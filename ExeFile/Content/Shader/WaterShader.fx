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
   
    //그려질 픽셀의 높이
    float uv_offset_y = (float) WATER_HEIGHT / g_client_resolution.y;
  
    //화면에서 해당 효과가 그려질 UV 좌표 구하기 => 픽셀 좌표/해상도
    float2 screen_uv = ps_input.position.xy / g_client_resolution;
  
    //v좌표 반전
    screen_uv.y = 1.0f - screen_uv.y;
    
    //이 부분은 좀 고려해봐야 함..
    screen_uv.y -= 2 * uv_offset_y;
   
    //굴절률 계산
    //Normal Map Texture를 이용
    float4 normal_map = NORMAL_TEXTURE.Sample(g_sampler1, ps_input.uv);
    //normal_map의 값은 (0 ~ 1)사이의 값인데 이를 요동치는 물결을 표현하기 위해 (-1, 1)로 변환
    float3 normal = (normal_map * 2.0f) - 1.0f;
    
    screen_uv = screen_uv + (normal.xy * REFRACT_SCALE);
    
    //실제 물결치는 움직임 구현
    //x축 방향으로 양옆으로 왔다갔다 하는 움직임
    float x = cos(ps_input.uv.y * 2 * PI + (g_accumulate_time * 4 * PI)) * 0.001f;
    screen_uv.x += x;
   
    //최종 UV 좌표로 Texture 샘플링
    ps_output_color = g_texture_0.Sample(g_sampler1, screen_uv);
    
    //샘플링한 픽셀의 색상을 파란색과 보간한 색상을 최종 색상으로 결정
    ps_output_color = lerp(ps_output_color, float4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f);
   
    return ps_output_color;
}
#endif