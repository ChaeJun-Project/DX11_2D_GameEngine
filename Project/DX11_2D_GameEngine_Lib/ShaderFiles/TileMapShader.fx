#ifndef TileMapShader
#define TileMapShader

#include "VertexStruct.fx"
#include "PixelStruct.fx"
#include "ConstantBuffer.fx"

#define TILE_COUNT_X          g_int_0 //Ÿ�� X�� ����
#define TILE_COUNT_Y          g_int_1 //Ÿ�� Y�� ����

//Tile Atlas Texture(�ִ� 8������)
#define TILE_ATLAS_TEXTURE_1    g_texture_0 //Ÿ�� ��Ʋ�� �ؽ�ó1
#define TILE_ATLAS_INFO_1       g_vector4_0 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_2    g_texture_1 //Ÿ�� ��Ʋ�� �ؽ�ó2
#define TILE_ATLAS_INFO_2       g_vector4_1 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_3    g_texture_2 //Ÿ�� ��Ʋ�� �ؽ�ó3
#define TILE_ATLAS_INFO_3       g_vector4_2 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_4    g_texture_3 //Ÿ�� ��Ʋ�� �ؽ�ó4
#define TILE_ATLAS_INFO_4       g_vector4_3 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_5    g_texture_4 //Ÿ�� ��Ʋ�� �ؽ�ó5
#define TILE_ATLAS_INFO_5       g_vector4_4 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_6    g_texture_5 //Ÿ�� ��Ʋ�� �ؽ�ó6
#define TILE_ATLAS_INFO_6       g_vector4_5 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_7    g_texture_6 //Ÿ�� ��Ʋ�� �ؽ�ó7
#define TILE_ATLAS_INFO_7       g_vector4_6 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

#define TILE_ATLAS_TEXTURE_8    g_texture_7 //Ÿ�� ��Ʋ�� �ؽ�ó8
#define TILE_ATLAS_INFO_8       g_vector4_7 //Ÿ�� ��Ʋ�� �ؽ�ó�� �ػ� & �� Ÿ�� ��Ʋ�� �ؽ�ó�� UV ��ǥ

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
    float4 ps_output_color = (float4) 0.0f;
   
    //Ÿ���� ��, �� ������ ���Ͽ� uv ��ǥ���� Ȯ��
    float2 expand_uv = ps_input.uv * float2(TILE_COUNT_X, TILE_COUNT_Y);
    //floor(float): float���� ���ų� ���� ���� ��ȯ
    //buffet_index.x: Ÿ�Ͽ��� �࿡ �ش��ϴ� �ε���(0���� ����)
    //buffet_index.y: Ÿ�Ͽ��� ���� �ش��ϴ� �ε���(0���� ����)
    int2 buffet_index = floor(expand_uv);
    
    //uv���� �� ���� ���� Ÿ���� �ε����� ����
    int current_tile_index = (TILE_COUNT_X * buffet_index.y + buffet_index.x);
    
 
    //���� Ÿ���� ��� ����
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
    
    int row = tile_image_index / tile_width_count; //�� ��° ������ ����
    int column = tile_image_index % tile_width_count; //�� ��° ������ ����
    
    //frac(float): float�� ���� �κ��� ������ �Ҽ��� ������ ��ȯ
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