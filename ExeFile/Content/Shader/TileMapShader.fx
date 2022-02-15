#ifndef TileMapShader
#define TileMapShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

#define TILE_COUNT_X          g_int_0 //타일 X축 개수
#define TILE_COUNT_Y          g_int_1 //타일 Y축 개수

//Tile Atlas Texture(최대 8개까지)
#define TILE_ATLAS_TEXTURE_1    g_texture_0 //타일 아틀라스 텍스처1
#define TILE_ATLAS_INFO_1       g_vector4_0 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_2    g_texture_1 //타일 아틀라스 텍스처2
#define TILE_ATLAS_INFO_2       g_vector4_1 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_3    g_texture_2 //타일 아틀라스 텍스처3
#define TILE_ATLAS_INFO_3       g_vector4_2 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_4    g_texture_3 //타일 아틀라스 텍스처4
#define TILE_ATLAS_INFO_4       g_vector4_3 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_5    g_texture_4 //타일 아틀라스 텍스처5
#define TILE_ATLAS_INFO_5       g_vector4_4 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_6    g_texture_5 //타일 아틀라스 텍스처6
#define TILE_ATLAS_INFO_6       g_vector4_5 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_7    g_texture_6 //타일 아틀라스 텍스처7
#define TILE_ATLAS_INFO_7       g_vector4_6 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

#define TILE_ATLAS_TEXTURE_8    g_texture_7 //타일 아틀라스 텍스처8
#define TILE_ATLAS_INFO_8       g_vector4_7 //타일 아틀라스 텍스처의 해상도 & 각 타일 아틀라스 텍스처의 UV 좌표

struct PerTileInfo
{
    int tile_atlas_texture_index;
    int tile_index;
    float2 left_top;
    float2 right_bottom;
};

StructuredBuffer<PerTileInfo> g_tile_buffer : register(t15);

//Vertex Shader
VertexColorTextureOutputType VS(VertexColorTexture vs_input)
{
    VertexColorTextureOutputType vs_output;
    
    //World -> View -> Projection 순서대로 곱해줘야 정상적인 위치 값이 나옴
    //mul(x, y)
    //x가 vector 값이면 행 벡터(Row-Vector)로 처리
    //y가 vector 값이면 열 벡터(Column-Vector)로 처리
    vs_output.position = mul(float4(vs_input.position, 1.0f), world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //Color값 복사
    vs_output.color = vs_input.color;
    
    //UV값 복사
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//Pixel Shader
float4 PS(VertexColorTextureOutputType ps_input) : SV_Target
{
    float4 ps_output_color = (float4) 0.0f;
   
    //타일의 행, 열 개수를 곱하여 uv 좌표값을 확장
    float2 expand_uv = ps_input.uv * float2(TILE_COUNT_X, TILE_COUNT_Y);
    //floor(float): float보다 같거나 작은 정수 반환
    //buffet_index.x: 타일에서 행에 해당하는 인덱스(0부터 시작)
    //buffet_index.y: 타일에서 열에 해당하는 인덱스(0부터 시작)
    int2 buffet_index = floor(expand_uv);
    
    //uv값과 을 통해 현재 타일의 인덱스를 구함
    int current_tile_index = (TILE_COUNT_X * buffet_index.y + buffet_index.x);
    
 
    //현재 타일의 출력 정보
    int tile_atlas_texture_index = g_tile_buffer[current_tile_index].tile_atlas_texture_index;
    int tile_image_index = g_tile_buffer[current_tile_index].tile_index;
    
    if (tile_atlas_texture_index == -1 || tile_image_index == -1)
    {
        return ps_output_color;
    }
    
    Texture2D tile_atlas_texture;
    float4 texture_info;
    
    switch (tile_atlas_texture_index)
    {
        case 0:
            {
                texture_info = TILE_ATLAS_INFO_1;
            }
            break;
        case 1:
          {
                texture_info = TILE_ATLAS_INFO_2;
            }
            break;
        case 2:
          {
                texture_info = TILE_ATLAS_INFO_3;
            }
            break;
        case 3:
          {
                texture_info = TILE_ATLAS_INFO_4;
            }
            break;
        case 4:
          {
                texture_info = TILE_ATLAS_INFO_5;
            }
            break;
        case 5:
          {
                texture_info = TILE_ATLAS_INFO_6;
            }
            break;
        case 6:
          {
                texture_info = TILE_ATLAS_INFO_7;
            }
            break;
        case 7:
          {
                texture_info = TILE_ATLAS_INFO_8;
            }
            break;
    }

    int tile_width_count = 1.0f / texture_info.z;
    
    int row = tile_image_index / tile_width_count; //몇 번째 행인지 구함
    int column = tile_image_index % tile_width_count; //몇 번째 열인지 구함
    
    //frac(float): float의 정수 부분을 제외한 소수점 값만을 반환
    expand_uv = frac(expand_uv);
    
    expand_uv *= texture_info.zw;
    
    expand_uv = texture_info.zw * float2(column, row) + expand_uv;
    
    switch (tile_atlas_texture_index)
    {
        case 0:
            {
                ps_output_color = TILE_ATLAS_TEXTURE_1.Sample(g_sampler1, expand_uv);
            }
            break;
        case 1:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_2.Sample(g_sampler1, expand_uv);
            }
            break;
        case 2:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_3.Sample(g_sampler1, expand_uv);
            }
            break;
        case 3:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_4.Sample(g_sampler1, expand_uv);
            }
            break;
        case 4:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_5.Sample(g_sampler1, expand_uv);
            }
            break;
        case 5:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_6.Sample(g_sampler1, expand_uv);
            }
            break;
        case 6:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_7.Sample(g_sampler1, expand_uv);
            }
            break;
        case 7:
          {
                ps_output_color = TILE_ATLAS_TEXTURE_8.Sample(g_sampler1, expand_uv);
            }
            break;
    }
 
    return ps_output_color;
}

#endif